/* Generated by re2c 0.13.5 on Thu Sep 24 18:23:18 2009 */
#line 1 "getoptint.re2c"
#include "pabidecl.h"
#include "pabstract.h"
#include "pparam.h"
#include "procore.h"

API_IMPL 
int
APICALL
tmplpro_get_int_option(struct tmplpro_param* param, const char *p, int* failure_ptr)
{
	register const char* YYMARKER;
	param->htp_errno = 0;
	if (failure_ptr) *failure_ptr=0;

#line 18 "<stdout>"
	{
		unsigned char yych;

		yych = (unsigned char)*p;
		switch (yych) {
		case 'd':	goto yy5;
		case 'f':	goto yy10;
		case 'g':	goto yy2;
		case 'l':	goto yy8;
		case 'm':	goto yy4;
		case 'n':	goto yy7;
		case 'p':	goto yy11;
		case 's':	goto yy9;
		case 't':	goto yy6;
		default:	goto yy12;
		}
yy2:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'l':	goto yy144;
		default:	goto yy3;
		}
yy3:
#line 31 "getoptint.re2c"
		{if (failure_ptr) *failure_ptr=ERR_PRO_INVALID_ARGUMENT; param->htp_errno=ERR_PRO_INVALID_ARGUMENT; return 0;}
#line 44 "<stdout>"
yy4:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'a':	goto yy132;
		default:	goto yy3;
		}
yy5:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'e':	goto yy114;
		default:	goto yy3;
		}
yy6:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'm':	goto yy101;
		default:	goto yy3;
		}
yy7:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'o':	goto yy90;
		default:	goto yy3;
		}
yy8:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'o':	goto yy73;
		default:	goto yy3;
		}
yy9:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'e':	goto yy45;
		case 't':	goto yy46;
		default:	goto yy3;
		}
yy10:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'i':	goto yy38;
		default:	goto yy3;
		}
yy11:
		yych = (unsigned char)*(YYMARKER = ++p);
		switch (yych) {
		case 'a':	goto yy13;
		default:	goto yy3;
		}
yy12:
		yych = (unsigned char)*++p;
		goto yy3;
yy13:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy15;
		default:	goto yy14;
		}
yy14:
		p = YYMARKER;
		goto yy3;
yy15:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'h':	goto yy16;
		default:	goto yy14;
		}
yy16:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy17;
		default:	goto yy14;
		}
yy17:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy18;
		default:	goto yy14;
		}
yy18:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy19;
		default:	goto yy14;
		}
yy19:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'k':	goto yy20;
		default:	goto yy14;
		}
yy20:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy21;
		default:	goto yy14;
		}
yy21:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy22;
		default:	goto yy14;
		}
yy22:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'v':	goto yy23;
		default:	goto yy14;
		}
yy23:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy24;
		default:	goto yy14;
		}
yy24:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy25;
		default:	goto yy14;
		}
yy25:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy26;
		default:	goto yy14;
		}
yy26:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy27;
		default:	goto yy14;
		}
yy27:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'b':	goto yy28;
		default:	goto yy14;
		}
yy28:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy29;
		default:	goto yy14;
		}
yy29:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy30;
		default:	goto yy14;
		}
yy30:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy31;
		default:	goto yy14;
		}
yy31:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy32;
		default:	goto yy14;
		}
yy32:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy33;
		default:	goto yy14;
		}
yy33:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'o':	goto yy34;
		default:	goto yy14;
		}
yy34:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'p':	goto yy35;
		default:	goto yy14;
		}
yy35:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy36;
		default:	goto yy14;
		}
yy36:
		++p;
#line 29 "getoptint.re2c"
		{return (int) param->path_like_variable_scope;}
#line 236 "<stdout>"
yy38:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy39;
		default:	goto yy14;
		}
yy39:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy40;
		default:	goto yy14;
		}
yy40:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy41;
		default:	goto yy14;
		}
yy41:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy42;
		default:	goto yy14;
		}
yy42:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy43;
		default:	goto yy14;
		}
yy43:
		++p;
#line 27 "getoptint.re2c"
		{return (int) param->filters;}
#line 271 "<stdout>"
yy45:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy52;
		default:	goto yy14;
		}
yy46:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy47;
		default:	goto yy14;
		}
yy47:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy48;
		default:	goto yy14;
		}
yy48:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy49;
		default:	goto yy14;
		}
yy49:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy50;
		default:	goto yy14;
		}
yy50:
		++p;
#line 26 "getoptint.re2c"
		{return (int) param->strict;}
#line 306 "<stdout>"
yy52:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy53;
		default:	goto yy14;
		}
yy53:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy54;
		default:	goto yy14;
		}
yy54:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'h':	goto yy55;
		default:	goto yy14;
		}
yy55:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy56;
		default:	goto yy14;
		}
yy56:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'p':	goto yy57;
		default:	goto yy14;
		}
yy57:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy58;
		default:	goto yy14;
		}
yy58:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy59;
		default:	goto yy14;
		}
yy59:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'h':	goto yy60;
		default:	goto yy14;
		}
yy60:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy61;
		default:	goto yy14;
		}
yy61:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'o':	goto yy62;
		default:	goto yy14;
		}
yy62:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'n':	goto yy63;
		default:	goto yy14;
		}
yy63:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy64;
		default:	goto yy14;
		}
yy64:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy65;
		default:	goto yy14;
		}
yy65:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'n':	goto yy66;
		default:	goto yy14;
		}
yy66:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy67;
		default:	goto yy14;
		}
yy67:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy68;
		default:	goto yy14;
		}
yy68:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'u':	goto yy69;
		default:	goto yy14;
		}
yy69:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'd':	goto yy70;
		default:	goto yy14;
		}
yy70:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy71;
		default:	goto yy14;
		}
yy71:
		++p;
#line 30 "getoptint.re2c"
		{return (int) param->search_path_on_include;}
#line 425 "<stdout>"
yy73:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'o':	goto yy74;
		default:	goto yy14;
		}
yy74:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'p':	goto yy75;
		default:	goto yy14;
		}
yy75:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy76;
		default:	goto yy14;
		}
yy76:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy77;
		default:	goto yy14;
		}
yy77:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'o':	goto yy78;
		default:	goto yy14;
		}
yy78:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'n':	goto yy79;
		default:	goto yy14;
		}
yy79:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy80;
		default:	goto yy14;
		}
yy80:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy81;
		default:	goto yy14;
		}
yy81:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'x':	goto yy82;
		default:	goto yy14;
		}
yy82:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy83;
		default:	goto yy14;
		}
yy83:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy84;
		default:	goto yy14;
		}
yy84:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'v':	goto yy85;
		default:	goto yy14;
		}
yy85:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy86;
		default:	goto yy14;
		}
yy86:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy87;
		default:	goto yy14;
		}
yy87:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy88;
		default:	goto yy14;
		}
yy88:
		++p;
#line 25 "getoptint.re2c"
		{return (int) param->loop_context_vars;}
#line 520 "<stdout>"
yy90:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy91;
		default:	goto yy14;
		}
yy91:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy92;
		default:	goto yy14;
		}
yy92:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'n':	goto yy93;
		default:	goto yy14;
		}
yy93:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy94;
		default:	goto yy14;
		}
yy94:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy95;
		default:	goto yy14;
		}
yy95:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'u':	goto yy96;
		default:	goto yy14;
		}
yy96:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'd':	goto yy97;
		default:	goto yy14;
		}
yy97:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy98;
		default:	goto yy14;
		}
yy98:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy99;
		default:	goto yy14;
		}
yy99:
		++p;
#line 24 "getoptint.re2c"
		{return (int) param->no_includes;}
#line 579 "<stdout>"
yy101:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'p':	goto yy102;
		default:	goto yy14;
		}
yy102:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy103;
		default:	goto yy14;
		}
yy103:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy104;
		default:	goto yy14;
		}
yy104:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'v':	goto yy105;
		default:	goto yy14;
		}
yy105:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy106;
		default:	goto yy14;
		}
yy106:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy107;
		default:	goto yy14;
		}
yy107:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy108;
		default:	goto yy14;
		}
yy108:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy109;
		default:	goto yy14;
		}
yy109:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy110;
		default:	goto yy14;
		}
yy110:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy111;
		default:	goto yy14;
		}
yy111:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy112;
		default:	goto yy14;
		}
yy112:
		++p;
#line 23 "getoptint.re2c"
		{return param->tmpl_var_case;}
#line 650 "<stdout>"
yy114:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'b':	goto yy115;
		case 'f':	goto yy116;
		default:	goto yy14;
		}
yy115:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'u':	goto yy129;
		default:	goto yy14;
		}
yy116:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy117;
		default:	goto yy14;
		}
yy117:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'u':	goto yy118;
		default:	goto yy14;
		}
yy118:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy119;
		default:	goto yy14;
		}
yy119:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 't':	goto yy120;
		default:	goto yy14;
		}
yy120:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy121;
		default:	goto yy14;
		}
yy121:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy122;
		default:	goto yy14;
		}
yy122:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy123;
		default:	goto yy14;
		}
yy123:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy124;
		default:	goto yy14;
		}
yy124:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy125;
		default:	goto yy14;
		}
yy125:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'p':	goto yy126;
		default:	goto yy14;
		}
yy126:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy127;
		default:	goto yy14;
		}
yy127:
		++p;
#line 28 "getoptint.re2c"
		{return param->default_escape;}
#line 734 "<stdout>"
yy129:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'g':	goto yy130;
		default:	goto yy14;
		}
yy130:
		++p;
#line 22 "getoptint.re2c"
		{return param->debug;}
#line 745 "<stdout>"
yy132:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'x':	goto yy133;
		default:	goto yy14;
		}
yy133:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy134;
		default:	goto yy14;
		}
yy134:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'i':	goto yy135;
		default:	goto yy14;
		}
yy135:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'n':	goto yy136;
		default:	goto yy14;
		}
yy136:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'c':	goto yy137;
		default:	goto yy14;
		}
yy137:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy138;
		default:	goto yy14;
		}
yy138:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'u':	goto yy139;
		default:	goto yy14;
		}
yy139:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'd':	goto yy140;
		default:	goto yy14;
		}
yy140:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'e':	goto yy141;
		default:	goto yy14;
		}
yy141:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy142;
		default:	goto yy14;
		}
yy142:
		++p;
#line 21 "getoptint.re2c"
		{return param->max_includes;}
#line 810 "<stdout>"
yy144:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'o':	goto yy145;
		default:	goto yy14;
		}
yy145:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'b':	goto yy146;
		default:	goto yy14;
		}
yy146:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy147;
		default:	goto yy14;
		}
yy147:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'l':	goto yy148;
		default:	goto yy14;
		}
yy148:
		yych = (unsigned char)*++p;
		switch (yych) {
		case '_':	goto yy149;
		default:	goto yy14;
		}
yy149:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'v':	goto yy150;
		default:	goto yy14;
		}
yy150:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'a':	goto yy151;
		default:	goto yy14;
		}
yy151:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 'r':	goto yy152;
		default:	goto yy14;
		}
yy152:
		yych = (unsigned char)*++p;
		switch (yych) {
		case 's':	goto yy153;
		default:	goto yy14;
		}
yy153:
		++p;
#line 20 "getoptint.re2c"
		{return param->global_vars;}
#line 869 "<stdout>"
	}
#line 32 "getoptint.re2c"

}

/*
 * Local variables:
 * mode: c
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

