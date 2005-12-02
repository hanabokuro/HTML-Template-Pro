#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "procore.h"
#include "pstack.h"
#include "pbuffer.h"
#include "proscope.h"
#include "expr.h"

#define HTML_TEMPLATE_BAD_TAG     0
#define HTML_TEMPLATE_FIRST_TAG_USED 1
#define HTML_TEMPLATE_TAG_VAR     1
#define HTML_TEMPLATE_TAG_INCLUDE 2
#define HTML_TEMPLATE_TAG_LOOP    3
#define HTML_TEMPLATE_TAG_IF      4
#define HTML_TEMPLATE_TAG_ELSE    5
#define HTML_TEMPLATE_TAG_UNLESS  6
#define HTML_TEMPLATE_LAST_TAG_USED  6

#define HTML_TEMPLATE_OPT_ESCAPE_NO   0
#define HTML_TEMPLATE_OPT_ESCAPE_HTML 1
#define HTML_TEMPLATE_OPT_ESCAPE_URL  2
#define HTML_TEMPLATE_OPT_ESCAPE_JS   3

#if defined(__GNUC__) && !(defined(PEDANTIC))
#define INLINE inline
#else /* !__GNUC__ */
#define INLINE 
#endif /* __GNUC__ */

static 
const char* const tagname[]={
    "Bad or unsupported tag", /* 0 */
    "var", "include", "loop", "if", "else", "unless"
};

static 
const char* const TAGNAME[]={
    "Bad or unsupported tag", /* 0 */
    "VAR", "INCLUDE", "LOOP", "IF", "ELSE", "UNLESS"
};

/* max offset to ensure we are not out of file when try <!--/  */
#define TAG_WIDTH_OFFSET 4

static int debuglevel=0;

static 
const char* const innerloopname[]={
  "", "first__", "last__", "inner__", "odd__", "counter__"
};

static 
const char* const INNERLOOPNAME[]={
  "", "FIRST__", "LAST__", "INNER__", "ODD__", "COUNTER__"
};

#define HTML_TEMPLATE_INNER_LOOP_VAR_FIRST   1
#define HTML_TEMPLATE_INNER_LOOP_VAR_LAST    2
#define HTML_TEMPLATE_INNER_LOOP_VAR_INNER   3
#define HTML_TEMPLATE_INNER_LOOP_VAR_ODD     4
#define HTML_TEMPLATE_INNER_LOOP_VAR_COUNTER 5

#define HTML_TEMPLATE_FIRST_INNER_LOOP 1
#define HTML_TEMPLATE_LAST_INNER_LOOP  5

static 
int 
try_inner_loop_variable (PSTRING name)
{ 
  int i;
  char* cur_pos;
  const char* pattern;
  const char* PATTERN;
  for (i=HTML_TEMPLATE_FIRST_INNER_LOOP; i<=HTML_TEMPLATE_LAST_INNER_LOOP; i++) {
    cur_pos=name.begin;
    pattern=innerloopname[i];
    PATTERN=INNERLOOPNAME[i];
    while (*pattern && cur_pos<name.endnext) {
      if (*pattern == *cur_pos || *PATTERN == *cur_pos) {
	pattern++;
	PATTERN++;
	cur_pos++;
      } else {
	break;
      }
    }
    if (cur_pos==name.endnext) {
      return i;
    }
  }
  return 0;
}

static 
PSTRING 
get_loop_context_vars_value (PSTRING name) {
  static char* FalseString="0";
  static char* TrueString ="1";
  static char buffer[20]; /* for snprintf %d */
  int loop;
  PSTRING retval={NULL,NULL};
  if (CurScopeLevel()>0 
      && name.endnext-name.begin>4
      && '_'==*(name.begin)
      && '_'==*(name.begin+1)
      ) { 
    /* we can meet loop variables here -- try it first */
    /* length of its name >4 */
    /* __first__ __last__ __inner__ __odd__ __counter__ */
    PSTRING shiftedname; /* (PSTRING) {name.begin+2,name.endnext} */
    shiftedname.begin=name.begin+2;
    shiftedname.endnext=name.endnext;
    switch (try_inner_loop_variable(shiftedname)) {
    case 0:  break;
    case HTML_TEMPLATE_INNER_LOOP_VAR_FIRST: 
      if (CurrentScope->loop==0) {  /* first__ */
	retval.begin=TrueString; retval.endnext=TrueString+1;
      } else {
	retval.begin=FalseString; retval.endnext=FalseString+1;
      }; break;
    case HTML_TEMPLATE_INNER_LOOP_VAR_LAST: 
      if (CurrentScope->loop==CurrentScope->maxloop) {
	retval.begin=TrueString; retval.endnext=TrueString+1;
      } else {
	retval.begin=FalseString; retval.endnext=FalseString+1;
      }; break;
    case HTML_TEMPLATE_INNER_LOOP_VAR_ODD: 
      if ((CurrentScope->loop%2)==0) {
	retval.begin=TrueString; retval.endnext=TrueString+1;
      } else {
	retval.begin=FalseString; retval.endnext=FalseString+1;
      }; break;
    case HTML_TEMPLATE_INNER_LOOP_VAR_INNER: 
      if (CurrentScope->loop>0 && CurrentScope->loop<CurrentScope->maxloop) {
	retval.begin=TrueString; retval.endnext=TrueString+1;
      } else {
	retval.begin=FalseString; retval.endnext=FalseString+1;
      }; break;
    case HTML_TEMPLATE_INNER_LOOP_VAR_COUNTER: 
      loop=CurrentScope->loop+1;
      snprintf(buffer,sizeof(buffer),"%d",loop);
      retval.begin=buffer; retval.endnext=buffer+strlen(buffer);
      break;
    }
  }
  return retval;
}

static 
int 
is_string(struct tmplpro_state *state, const char* pattern,const char* PATTERN)
{
  char* cur_pos=state->cur_pos;
  while (*pattern && cur_pos<state->next_to_end) {
    if (*pattern == *cur_pos || *PATTERN == *cur_pos) {
      pattern++;
      PATTERN++;
      cur_pos++;
    } else {
      return 0;
    }
  }
  if (cur_pos>=state->next_to_end) return 0;
  state->cur_pos=cur_pos;
  return 1;
}


static 
INLINE 
void 
jump_over_space(struct tmplpro_state *state)
{
  while (isspace(*(state->cur_pos)) && state->cur_pos<state->next_to_end) {state->cur_pos++;};
}

static 
INLINE
void 
jump_to_char(struct tmplpro_state *state, char c)
{
  while (c!=*(state->cur_pos) && state->cur_pos<state->next_to_end) {state->cur_pos++;};
}

static 
PSTRING 
escape_pstring (PSTRING pstring, int escapeopt) {
  char* buf=pbuffer_resize(2*(pstring.endnext-pstring.begin+1));
  char* curpos=pstring.begin;
  size_t offset=0;
  size_t buflen=pbuffer_size();

  switch (escapeopt) {
  case HTML_TEMPLATE_OPT_ESCAPE_HTML:
    while (curpos<pstring.endnext) {
      char curchar=*curpos++;
      int bufdelta=1;
      if (offset>=buflen) {
	buf=pbuffer_resize(2*offset);
	buflen=pbuffer_size();
      }
      switch (curchar) {
	/* straight from the CGI.pm bible. (HTML::Template) */
      case '&' : bufdelta=5; strncpy(buf+offset, "&amp;", bufdelta);break;
      case '"' : bufdelta=6; strncpy(buf+offset, "&quot;",bufdelta);break;
      case '>' : bufdelta=4; strncpy(buf+offset, "&gt;",  bufdelta);break;
      case '<' : bufdelta=4; strncpy(buf+offset, "&lt;",  bufdelta);break;
      case '\'': bufdelta=5; strncpy(buf+offset, "&#39;", bufdelta);break;
      default: *(buf+offset)=curchar;
      }
      offset+=bufdelta;
    }
    break;
  case HTML_TEMPLATE_OPT_ESCAPE_JS:
    while (curpos<pstring.endnext) {
      char curchar=*curpos++;
      int bufdelta=1;
      if (offset>=buflen) {
	buf=pbuffer_resize(2*offset);
	buflen=pbuffer_size();
      }
      switch (curchar) {
      case '\\' : bufdelta=2; strncpy(buf+offset, "\\\\", bufdelta);break;
      case '"'  : bufdelta=2; strncpy(buf+offset, "\\\"",bufdelta);break;
      case '\'' : bufdelta=2; strncpy(buf+offset, "\\'",bufdelta);break;
      case '\n' : bufdelta=2; strncpy(buf+offset, "\\n",bufdelta);break;
      case '\r' : bufdelta=2; strncpy(buf+offset, "\\r",bufdelta);break;
      default: *(buf+offset)=curchar;
      }
      offset+=bufdelta;
    }
    break;
  case HTML_TEMPLATE_OPT_ESCAPE_URL: 
    while (curpos<pstring.endnext) {
      char curchar=*curpos++;
      int bufdelta=1;
      if (offset>=buflen) {
	buf=pbuffer_resize(2*offset);
	buflen=pbuffer_size();
      }
      /* 
       * # do the translation (RFC 2396 ^uric)
       * s!([^a-zA-Z0-9_.\-])!sprintf('%%%02X', $_)
       */
      if ((curchar>='a' && curchar<='z') ||
	  (curchar>='A' && curchar<='Z') ||
	  (curchar>='0' && curchar<='9') ||
	  curchar=='_' || curchar=='.' || curchar=='\\' || curchar=='-'
	  ) 
	*(buf+offset)=curchar;
      else {
	bufdelta=3; sprintf(buf+offset,"%%%02X",(int) curchar);
      }
      offset+=bufdelta;
    }
    break;
  default : return pstring;
  }
  return (PSTRING) {buf,buf+offset};
}

void 
_tmpl_log_state (struct tmplpro_state *state, int level)
{
  tmpl_log(NULL,level, "HTML::Template::Pro:in %cTMPL_%s at pos %d:",
	  (state->is_tag_closed ? '/' : ' '), 
	   (state->tag>HTML_TEMPLATE_BAD_TAG && state->tag <=HTML_TEMPLATE_LAST_TAG_USED) ? TAGNAME[state->tag] : "", 
	   state->tag_start - state->top);
}

static 
void 
tag_handler_var (struct tmplpro_state *state, PSTRING name, PSTRING defvalue, int escapeopt)
{
  PSTRING varvalue ={NULL, NULL};
  /* registering variable could be inside GetVarFuncPtr */
  if (! state->is_visible) return;
  if (state->is_expr) {
    varvalue=parse_expr(name, state->param);
  } else 
    if (state->param->loop_context_vars) {
      varvalue=get_loop_context_vars_value(name);
    }
  if (varvalue.begin==NULL) {
    varvalue=(state->param->GetVarFuncPtr)(state->param, name);
  }
  if (debuglevel>=TMPL_LOG_DEBUG) {
      if (varvalue.begin!=NULL) {
      tmpl_log(state,TMPL_LOG_DEBUG,"variable value = %.*s\n",varvalue.endnext-varvalue.begin,varvalue.begin);
    } else {
      tmpl_log(state,TMPL_LOG_DEBUG,"variable value = UNDEF\n");
    }
  }
  if (varvalue.begin==NULL && defvalue.begin!=defvalue.endnext) {
    varvalue=defvalue;
  }
  if (escapeopt!=HTML_TEMPLATE_OPT_ESCAPE_NO) {
    varvalue=escape_pstring(varvalue,escapeopt);
  }
  (state->param->WriterFuncPtr)(varvalue.begin,varvalue.endnext);
}

static 
void 
tag_handler_include (struct tmplpro_state *state, PSTRING name, PSTRING defvalue)
{
  struct tmplpro_param* param;
  char* filename;
  const char* selfpath;
  int x;
  PSTRING varvalue=name;
  if (! state->is_visible) return;
  param=state->param;
  if (param->no_includes) {
    tmpl_log(state,TMPL_LOG_ERROR, "HTML::Template::Pro : Illegal attempt to use TMPL_INCLUDE in template file : (no_includes => 1)");
    return;
  }
  if (param->max_includes && param->max_includes < param->cur_includes) return;
  param->cur_includes++;

  if (state->is_expr) {
    varvalue=parse_expr(name, param);
  } else 
    if (varvalue.begin==varvalue.endnext && defvalue.begin!=defvalue.endnext)
      varvalue=defvalue;
  /* pstrdup */
  filename =(char*) malloc(varvalue.endnext-varvalue.begin+1);
  for (x=0;x<varvalue.endnext-varvalue.begin;x++) {
    *(filename+x)=*(varvalue.begin+x);
  }
  *(filename+(varvalue.endnext-varvalue.begin))=0;
  /* end pstrdup */
  selfpath=param->selfpath; /* saving current file name */
  exec_tmpl (filename, param);
  free (filename);
  param->selfpath=selfpath;
  param->cur_includes--; 
  return;
}

static 
int 
is_var_true(struct tmplpro_state *state, PSTRING name) 
{
  register int ifval=-1;
  if (state->is_expr) {
    ifval=is_pstring_true(parse_expr(name, state->param));
  } else
    if (state->param->loop_context_vars) {
      PSTRING loop_var=get_loop_context_vars_value(name);
      if (loop_var.begin!=NULL) {
	ifval=is_pstring_true(loop_var);
      }
    }
  if (ifval==-1) ifval=(state->param->IsVarTrueFuncPtr)(state->param, name);
  return ifval;
}

static 
void 
tag_handler_if (struct tmplpro_state *state, PSTRING name)
{
  struct pstack_entry iftag;
  iftag.tag=HTML_TEMPLATE_TAG_IF;
  iftag.vcontext=state->is_visible;
  iftag.position=state->cur_pos; /* unused */
  if (is_var_true(state,name)) {
    iftag.value=1;
    /* state->is_visible is not touched */
  } else {
    iftag.value=0;
    state->is_visible=0;
  }
  pstack_push(iftag);
  if (debuglevel>3) tmpl_log(state,TMPL_LOG_DEBUG2,"tag_handler_if:visible context =%d value=%d ",iftag.vcontext,iftag.value);
}

static 
void 
tag_handler_unless (struct tmplpro_state *state, PSTRING name)
{
  struct pstack_entry iftag;
  iftag.tag=HTML_TEMPLATE_TAG_UNLESS;
  iftag.vcontext=state->is_visible;
  iftag.position=state->cur_pos; /* unused */
  if (is_var_true(state,name)) {
    iftag.value=0;
    pstack_push(iftag);
    state->is_visible=0;
  } else {
    iftag.value=1;
    pstack_push(iftag);
    /* state->is_visible is not touched */
  }
  if (debuglevel>3) tmpl_log(state,TMPL_LOG_DEBUG2,"unless:visible context =%d value=%d ",iftag.vcontext,iftag.value);
}

static 
INLINE
int
test_stack (int tag)
{
  return (pstack_notempty() && (pstack_head()->tag==tag));
}

static 
void 
tag_stack_debug  (struct tmplpro_state *state, int stack_tag_type)
{
  if (stack_tag_type) {
    if (pstack_notempty()) {
      struct pstack_entry iftag=pstack_top();
      if (iftag.tag!=stack_tag_type) {
	tmpl_log(state,TMPL_LOG_ERROR, "ERROR: tag mismatch with %s\n",TAGNAME[iftag.tag]);
      }
    } else {
      tmpl_log(state,TMPL_LOG_ERROR, "ERROR: opening tag %s not found\n",TAGNAME[stack_tag_type]);
    }
  }
}

static 
void 
tag_handler_closeif (struct tmplpro_state *state)
{
  struct pstack_entry iftag;
  if (! test_stack(HTML_TEMPLATE_TAG_IF)) {
    tag_stack_debug(state,HTML_TEMPLATE_TAG_IF);
    return;
  }
  iftag=pstack_pop();
  if (0==state->is_visible) state->last_processed_pos=state->cur_pos;
  state->is_visible=iftag.vcontext;
}

static 
void 
tag_handler_closeunless (struct tmplpro_state *state)
{
  struct pstack_entry iftag;
  if (! test_stack(HTML_TEMPLATE_TAG_UNLESS)) {
    tag_stack_debug(state,HTML_TEMPLATE_TAG_UNLESS);
    return;
  }
  iftag=pstack_pop();
  if (0==state->is_visible) state->last_processed_pos=state->cur_pos;
  state->is_visible=iftag.vcontext;
}

static 
void 
tag_handler_else (struct tmplpro_state *state)
{
  struct pstack_entry iftag;
  if (! test_stack(HTML_TEMPLATE_TAG_IF) && 
      ! test_stack(HTML_TEMPLATE_TAG_UNLESS)) {
    tag_stack_debug(state,HTML_TEMPLATE_TAG_ELSE);
    return;
  }
  iftag=pstack_top();
  if (0==state->is_visible) state->last_processed_pos=state->cur_pos;
  if (iftag.value) {
    state->is_visible=0;
  } else if (1==iftag.vcontext) {
    state->is_visible=1;
  }
  if (debuglevel>3) tmpl_log(state,TMPL_LOG_DEBUG2,"else:(pos %d) visible:context =%d, set to %d ",iftag.position - state->top,iftag.vcontext,state->is_visible);
}

static 
int 
next_loop (struct tmplpro_state* state) {
#ifdef DEBUG
  tmpl_log(state,TMPL_LOG_DEBUG2,"next_loop:before NextLoopFuncPtr\n");
#endif
  if (++CurrentScope->loop >CurrentScope->maxloop || !(state->param->NextLoopFuncPtr)(state)) {
    PopScope();
    return 0;
  } else return 1;
}

static 
void 
tag_handler_loop (struct tmplpro_state *state, PSTRING name)
{
  struct pstack_entry iftag;
  iftag.tag=HTML_TEMPLATE_TAG_LOOP;
  iftag.vcontext=state->is_visible;
  iftag.value=0;
  iftag.position=state->cur_pos; /* loop start - to restore in </tmpl_loop> */
#ifdef DEBUG
  tmpl_log(state,TMPL_LOG_DEBUG2,"tag_handler_loop:before InitLoopFuncPtr\n");
#endif
  if (state->is_visible && (*state->param->InitLoopFuncPtr)(state,name) && next_loop(state)) {
    iftag.value=1; /* the loop is non - empty */
  } else {
    /* empty loop is equal to <if false> ... </if> */
    state->is_visible=0;
  }
#ifdef DEBUG
  tmpl_log(state,TMPL_LOG_DEBUG2,"tag_handler_loop:after InitLoopFuncPtr\n");
#endif
  pstack_push(iftag);
}

static 
void 
tag_handler_closeloop (struct tmplpro_state *state)
{
  struct pstack_entry iftag;
  if (! test_stack(HTML_TEMPLATE_TAG_LOOP)) {
    tag_stack_debug(state,HTML_TEMPLATE_TAG_LOOP);
    return;
  }
  iftag=pstack_top();
  if (iftag.value==1 && next_loop(state)) {
    /* continue loop */
    state->cur_pos=iftag.position;
    state->last_processed_pos=iftag.position;
    return;
  } else {
    /* finish loop */
    iftag=pstack_pop();
    state->is_visible=iftag.vcontext;
    state->last_processed_pos=state->cur_pos;
  }
}

static 
void 
tag_handler_unknown (struct tmplpro_state *state)
{
  tmpl_log(state,TMPL_LOG_ERROR,"tag_handler_unknown: unknown tag\n");
}

static 
PSTRING 
read_tag_parameter_value (struct tmplpro_state *state)
{
  PSTRING modifier_value;
  char cur_char;
  char quote_char=0;
  register char* cur_pos;
  register char* next_to_end=state->next_to_end;
  jump_over_space(state);
  cur_pos=state->cur_pos;
  cur_char=*(cur_pos);
  if (('"'==cur_char) || ('\''==cur_char)) {
    quote_char=*(cur_pos);
    cur_pos++;
  }
  modifier_value.begin=cur_pos;
  cur_char=*(cur_pos);
  if (quote_char) {
    while (quote_char!=cur_char 
#ifdef COMPAT_ON_BROKEN_QUOTE
/* compatibility mode; HTML::Template doesn't allow '>' inside quotes */
	   && ('>' != quote_char)
#endif
	   && cur_pos<next_to_end) {
      cur_pos++;
      cur_char=*(cur_pos);
    }
  } else {
    while ('>'!=cur_char && ! isspace(cur_char) && cur_pos<next_to_end) {
      cur_pos++;
      cur_char=*(cur_pos);
    }
  }
  if (cur_pos>=next_to_end) {
    tmpl_log(state,TMPL_LOG_ERROR,"quote char %c at pos %d is not terminated\n",quote_char,state->cur_pos-state->top);
    modifier_value.endnext=modifier_value.begin;
    jump_over_space(state);
    return modifier_value;
  }
  modifier_value.endnext=cur_pos;
  if (quote_char) {
    if (quote_char==*cur_pos) {
      cur_pos++;
    } else {
      tmpl_log(state,TMPL_LOG_ERROR,"found %c instead of end quote %c at pos %d\n",*cur_pos,quote_char,cur_pos - state->top);
    }
  }
  state->cur_pos=cur_pos;
  /* tmpl_log(state,TMPL_LOG_DEBUG2," at pos %d",state->cur_pos-state->top); */
  jump_over_space(state);
  return modifier_value;
}

static 
int 
try_tag_parameter (struct tmplpro_state *state,const char *modifier,const char *MODIFIER)
{
  jump_over_space(state);
  if (is_string(state, modifier, MODIFIER)) {
    jump_over_space(state);
    if ('='==*(state->cur_pos)) {
      state->cur_pos++;
    } else {
      tmpl_log(state,TMPL_LOG_ERROR,"found %c instead of = at pos %d\n",*state->cur_pos,state->cur_pos-state->top);
    }
    jump_over_space(state);
    return 1;
  }
  return 0;
}

static 
void 
try_tmpl_var_options (struct tmplpro_state *state, PSTRING* OptEscape, PSTRING* OptDefault)
{
  static const char* escapeopt="escape";
  static const char* ESCAPEOPT="ESCAPE";
  static const char* defaultopt="default";
  static const char* DEFAULTOPT="DEFAULT";
  if (try_tag_parameter(state, escapeopt, ESCAPEOPT)) {
    *OptEscape=read_tag_parameter_value(state);
    tmpl_log(state,TMPL_LOG_DEBUG, "found option ESCAPE at pos %d\n",state->cur_pos-state->top);
  }
  if (try_tag_parameter(state, defaultopt, DEFAULTOPT)) {
    *OptDefault=read_tag_parameter_value(state);
    tmpl_log(state,TMPL_LOG_DEBUG, "found option DEFAULT at pos %d\n",state->cur_pos-state->top);
  }
}


static 
void 
process_tmpl_tag(struct tmplpro_state *state)
{
  char* tag_start=state->tag_start;
  int is_tag_closed=state->is_tag_closed;

  int tag_type=HTML_TEMPLATE_BAD_TAG;
  static const char* nameopt="name";
  static const char* NAMEOPT="NAME";
  static const char* expropt="expr";
  static const char* EXPROPT="EXPR";

  PSTRING OptName  = {NULL,NULL};
  PSTRING OptDefault={NULL,NULL};
  PSTRING OptEscape= {NULL,NULL};

  int i;
  for (i=HTML_TEMPLATE_FIRST_TAG_USED; i<=HTML_TEMPLATE_LAST_TAG_USED; i++) {
    if (is_string(state, tagname[i], TAGNAME[i])) {
      tag_type=i;
      state->tag=tag_type;
      if (debuglevel) {
	if (is_tag_closed) {
	  tmpl_log(NULL,TMPL_LOG_DEBUG, "found </TMPL_%s> at pos %d\n",TAGNAME[i],state->cur_pos-state->top);
	} else {
	  tmpl_log(NULL,TMPL_LOG_DEBUG, "found <TMPL_%s> at pos %d\n",TAGNAME[i],state->cur_pos-state->top);
	}
      }
      break;
    }
  }
  if (HTML_TEMPLATE_BAD_TAG==tag_type) {
    tmpl_log(state,TMPL_LOG_ERROR, "found BAD at pos %d ",state->cur_pos-state->top);
    /* TODO: flush its data ---  */
    state->cur_pos++;
    return;
  }

  if (is_tag_closed && (
			tag_type==HTML_TEMPLATE_TAG_ELSE
			|| tag_type==HTML_TEMPLATE_TAG_INCLUDE
			|| tag_type==HTML_TEMPLATE_TAG_VAR
)) {
    tmpl_log(state,TMPL_LOG_ERROR, "incorrect tag </TMPL_%s> at pos %d\n",TAGNAME[tag_type],state->cur_pos-state->top);
  }

  if (is_tag_closed || tag_type==HTML_TEMPLATE_TAG_ELSE) {
    /* tag has no parameter */
  } else {
    /* reading parameter */
    state->is_expr=0;
    if (tag_type==HTML_TEMPLATE_TAG_VAR
	 || tag_type==HTML_TEMPLATE_TAG_INCLUDE
	) {
      try_tmpl_var_options(state,&OptEscape,&OptDefault);
    }

    if (try_tag_parameter(state, expropt, EXPROPT)) {
      state->is_expr=1;
    } else {
      try_tag_parameter(state, nameopt, NAMEOPT);
    }
    OptName=read_tag_parameter_value(state);

    if (tag_type==HTML_TEMPLATE_TAG_VAR
	|| tag_type==HTML_TEMPLATE_TAG_INCLUDE
	) {
      try_tmpl_var_options(state,&OptEscape,&OptDefault);
    }
  }

  if (state->is_tag_commented) {
    /* try read comment end */
    /* jump_over_space(state); it should be already done :( */
    jump_over_space(state);
    if (state->cur_pos<state->next_to_end-2 && '-'==*(state->cur_pos) && '-'==*(state->cur_pos+1)) {
      state->cur_pos+=2;
    }
  }
  if ('>'==*(state->cur_pos)) {
    state->cur_pos++;
  } else {
    tmpl_log(state,TMPL_LOG_ERROR,"end tag:found %c instead of > at pos %d\n",*state->cur_pos,state->cur_pos-state->top);
  }
  /* flush run chars (if in SHOW mode) */
  if (state->is_visible) {
    (state->param->WriterFuncPtr)(state->last_processed_pos,tag_start);
    state->last_processed_pos=state->cur_pos;
  }
  /* TODO: call tag_specific handler by array of handlers 
     var_tag_handler(..) */
  if (is_tag_closed) {
    switch (tag_type) {
    case HTML_TEMPLATE_TAG_IF:	tag_handler_closeif(state);break;
    case HTML_TEMPLATE_TAG_UNLESS:	tag_handler_closeunless(state);break;
    case HTML_TEMPLATE_TAG_LOOP:	tag_handler_closeloop(state);break;
    default:	tag_handler_unknown(state);break;
    
    }
  } else {
    int escape = HTML_TEMPLATE_OPT_ESCAPE_NO;
    if (tag_type!= HTML_TEMPLATE_TAG_INCLUDE &&
	0==state->is_expr &&
	0==state->param->case_sensitive) OptName=lowercase_pstring(OptName);
    switch (tag_type) {
    case HTML_TEMPLATE_TAG_VAR:	
      if (OptEscape.begin!=OptEscape.endnext) {
	switch (*OptEscape.begin) {
	case '1': case 'H': case 'h': 	/* HTML*/
	  escape = HTML_TEMPLATE_OPT_ESCAPE_HTML;
	  break;
	case 'U': case 'u': 		/* URL */
	  escape = HTML_TEMPLATE_OPT_ESCAPE_URL;
	  break;
	case 'J': case 'j':		/* JS  */
	  escape = HTML_TEMPLATE_OPT_ESCAPE_JS;
	  break;
	case '0': 
	  escape = HTML_TEMPLATE_OPT_ESCAPE_NO;
	  break;
	default:
	  tmpl_log(state,TMPL_LOG_ERROR, " unsupported value of ESCAPE=%.*s\n",OptEscape.endnext-OptEscape.begin,OptEscape.begin);
	}
      }
      tag_handler_var(state,OptName,OptDefault,escape);
      break;
    case HTML_TEMPLATE_TAG_IF:	tag_handler_if(state,OptName);	break;
    case HTML_TEMPLATE_TAG_UNLESS:	tag_handler_unless(state,OptName);break;
    case HTML_TEMPLATE_TAG_ELSE:	tag_handler_else(state);	break;
    case HTML_TEMPLATE_TAG_LOOP:	tag_handler_loop(state,OptName);break;
    case HTML_TEMPLATE_TAG_INCLUDE:	tag_handler_include(state,OptName,OptDefault);break;
    default:	tag_handler_unknown(state);break;
    }
  }

}

static 
void 
process_state (struct tmplpro_state * state)
{
  static const char* metatag="tmpl_";
  static const char* METATAG="TMPL_";
  flag is_tag_closed;
  flag is_tag_commented;
  char* last_safe_pos=state->next_to_end-TAG_WIDTH_OFFSET;
  pbuffer_init();
  while (state->cur_pos < last_safe_pos) {
    register char* cur_pos=state->cur_pos;
    while (cur_pos < last_safe_pos && '<'!=*(cur_pos++)) {};
    if (cur_pos >= last_safe_pos) break;
    state->tag_start=cur_pos-1;
    is_tag_closed=0;
    is_tag_commented=0;
    state->cur_pos=cur_pos;
    if (('!'==*(cur_pos)) && ('-'==*(cur_pos+1)) && ('-'==*(cur_pos+2))) {
      state->cur_pos+=3;
      jump_over_space(state);
    is_tag_commented=1;
    }
    if ('/'==*(state->cur_pos)) {
      state->cur_pos++;
      is_tag_closed=1;
    }
    if (is_string(state,metatag,METATAG)) {
      state->is_tag_commented=is_tag_commented;
      state->is_tag_closed=is_tag_closed;
      process_tmpl_tag(state);
    }
  }
  (state->param->WriterFuncPtr)(state->last_processed_pos,state->next_to_end);

  pbuffer_free();
  if (debuglevel) tmpl_log(NULL,TMPL_LOG_DEBUG,"process_state:finished\n");
}

static 
void 
init_state (struct tmplpro_state *state, struct tmplpro_param *param)
{
  debuglevel=param->debug;
  tmpl_log_set_level(debuglevel);
  /* initializing state */
  state->param=param;
  state->last_processed_pos=state->top;
  state->cur_pos=state->top;
  state->is_visible=1;
}

/* for mmap_load_file & mmap_unload_file */
#include "loadfile.inc"

int 
exec_tmpl (const char* filename, struct tmplpro_param *param)
{
  struct tmplpro_state state;
  int mmapstatus;
  PSTRING memarea;
  /* 
   * param->selfpath is path to upper level template 
   * (or NULL in toplevel) which called <include filename>.
   * we use it to calculate filepath for filename.
   * Then filename becames upper level template for its <include>.
   */
  const char* filepath=(param->FindFileFuncPtr)(filename, param->selfpath); 
  param->selfpath=filepath;
  if (param->filters) memarea=(param->LoadFileFuncPtr)(filepath);
  else memarea=mmap_load_file(filepath);
  if (memarea.begin == NULL) {
    /* param-> */
    return ERR_PRO_CANT_OPEN_FILE;
  }
  state.top =memarea.begin;
  state.next_to_end=memarea.endnext;
  if (memarea.begin < memarea.endnext) {
    /* to avoid crash with empty file */
    init_state(&state,param);
    tmpl_log(&state,TMPL_LOG_DEBUG, "exec_tmpl: loading %s\n",filename);
    process_state(&state);
  }
  /* destroying */
  if (param->filters) mmapstatus=(param->UnloadFileFuncPtr)(memarea);
  else mmapstatus=mmap_unload_file(memarea);
  return 0;
}

int 
exec_tmpl_from_memory (PSTRING memarea, struct tmplpro_param *param)
{
  struct tmplpro_state state;
  param->selfpath=NULL; /* no upper file */
  state.top = memarea.begin;
  state.next_to_end=memarea.endnext;
  if (memarea.begin == memarea.endnext) return 0;
  init_state(&state,param);
  process_state(&state);
  return 0;
}

void 
procore_init()
{
  /* to save time on malloc/free at each included template */
  pbuffer_init();
  Scope_init();
  pstack_init();
  /* pstack_selftest(); */
  expr_init();
}

void 
procore_done()
{
  pstack_free();
  Scope_free();
  pbuffer_free();
  expr_free();
}

/* internal initialization of struct tmplpro_param */
void 
param_init(struct tmplpro_param* param)
{
  /* filling initial struct tmplpro_param with 0 */
  memset (param, 0, sizeof(struct tmplpro_param));
  /* current level of inclusion */
  /* param->cur_includes=0; */
  /* not to use external file loader */
  /* param->filters=0; */
  /* param->selfpath=NULL;
     param->ExprFuncHash=NULL;
     param->ExprFuncArglist=NULL;
  */
}

#include "pstack.inc"

/*
 * Local Variables:
 * mode: c 
 * coding: cp1251
 * End: 
 */
