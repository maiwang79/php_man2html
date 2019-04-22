/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "man/man2html/man2html.h"

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_man2html.h"

/* If you declare any globals in php_man2html.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(man2html)
*/

/* True global resources - no need for thread safety here */
static int le_man2html;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("man2html.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_man2html_globals, man2html_globals)
    STD_PHP_INI_ENTRY("man2html.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_man2html_globals, man2html_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */


/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_man2html_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_man2html_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP7.", "man2html", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(man_main){
  char *arg = NULL;
  size_t arg_len;
  zend_string *strg;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
      return;
  }

  php_printf("Content-type: text/html\n\n");

  #define XTRA 5

  char *buf = arg;
  int l = arg_len;

  scaninbuff = 0;
  output_possible = 1;
  buffmax = 1024;
while(strlen(arg)*1.1 > buffmax){
    buffmax = buffmax + 1024*2;
}
  returnbuffer = (char*)malloc(buffmax * sizeof(char));
  if (returnbuffer == NULL){
       php_printf("realloc fail");
       RETURN_FALSE;
  }

    buf[0] = '\n';
    buf[l+1] = '\n';
    buf[l+2] = buf[l+3] = 0;

    scan_troff(buf+1,0,NULL);
    dl_down();
    out_html(change_to_font(0));
    out_html(change_to_size(0));
    if (!fillout) {
	fillout=1;
	out_html("</PRE>");
    }
    out_html(NEWLINE);

    zend_string *zresult;
    char *resulthtml = 0;

    if (output_possible) {
	/* &nbsp; for mosaic users */
	out_html("<HR>\n<A NAME=\"index\">&nbsp;</A><H2>Index</H2>\n<DL>\n");
	//php_printf("%s", manidx);
	if (subs) out_html("</DL>\n");
	out_html("</DL>\n");
	print_sig();
	out_html("</BODY>\n</HTML>\n"); 

        //add_links(outbuffer);

        //php_printf("return var %s",returnbuffer);
        //resulthtml = outbuffer;
	zresult = strpprintf(0,"%s",returnbuffer);
    }
    else if(scaninbuff) {
        php_printf("output is %s",buffer);
        //resulthtml = buffer;
        zresult = strpprintf(0,"%s",buffer);
	//add_links(buffer);
    }

  //php_printf("buffer1 %i,buffer2 %i",output_possible,scaninbuff);
  //php_printf("buffer1_len %d-%d",sizeof(outbuffer),strlen(outbuffer));

  RETURN_STR(zresult);
}

PHP_FUNCTION(man_scan_troff){
  char *arg = NULL;
  size_t arg_len;
  zend_string *strg;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
      return;
  }

  char text[] = "";
  strcpy(text,arg);
  
  //char **r= NULL;
  char *theresult = 0;
  char * result = 0;
  char * strreturn = 0;
  strreturn = scan_troff(text,0,&theresult);

  php_printf("scan troff success %s return %s \n",theresult,strreturn);

  if (scaninbuff)
     result=buffer;
  else if (output_possible)
     result=outbuffer;

  php_printf("result is %s \n",result);

  char *realresult = 0;
  zend_string *zresult= strpprintf(0,"scan_troff result: %s",result);
  php_printf("strg safety result %s \n",result);

  //char text[] = "";
  //strcpy(text,arg)
  RETURN_STR(zresult);

}

PHP_FUNCTION(man_scan_escape){
  char *arg = NULL;
  size_t arg_len;
  zend_string *strg;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
      return;
  }

  char *result = scan_escape(arg);
  char *r =0;
  int str_len = spprintf(&r,0,"%s",result);

  zend_string *zresult= strpprintf(0,"scan_escape result: %s",result);
  //php_printf("strg safety result %s \n",result);
  RETURN_STR(zresult);

}

PHP_FUNCTION(man_scan_request){
  char *arg = NULL;
  size_t arg_len;
  zend_string *strg;

  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
      return;
  }

  char *theresult = scan_request(arg);
  php_printf("scan request param %s ",arg);
  php_printf("scan request rsult %s ",theresult);
  char *r =0;
  int str_len = spprintf(&r,0,"%s",theresult);
  RETURN_STRINGL(r,str_len);

}


PHP_FUNCTION(mantest){
  char *arg = NULL;
  size_t arg_len,len;
  zend_string *strg;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
      return;
  }

char text[]="";
strcpy(text,arg);

int n = sizeof(arg);
//c = expand_char(58);
//fill_words(str,words,maxn,&n,'\n');

//char str1[] = ".RB [\\- aABcdDeEfFghjklLmoqrRSTuvVwXyz!@$ ]";

char *c1 = text;
c1=scan_escape(c1);

php_printf("\nscan_escape get %s <hr/>\n\n",c1);


/*

        char eow = '\n';
	char *s = str;
	int backslash = 0;
	int skipspace = 0;	/* 1 if space is not end-of-word * /

	n = 0;
	words[n] = s;
	while (*s && (*s != '\n' || backslash)) {
		if (!backslash) {
			if (*s == '"') {
				//*s = '\a';
				skipspace = !skipspace;
			} else if (*s == escapesym) {
				backslash = 1;
			} else if ((*s == ' ' || *s == '\t') && !skipspace) {
				//*s = eow;
				if (words[n] != s && n < maxn-1)
					n++;
				words[n] = s+1; 
				//text = strcat(text,"str=");
				//text = strcat(text,s);
				php_printf("-%i-%s--\r\n",n,words[n]);
			}
		} else {
			if (*s == '"') {
				s--;
				*s = eow;
				if (words[n] != s && n < maxn-1)
					 n++;
				s++;
				while (*s && *s != '\n') s++;
				words[n] = s;
				s--;
			}
			backslash = 0;
		}
		s++;
	}
	if (s != words[n])
		n++;
*/


zend_string *result;

//  c++;
  int nr = 58;
//php_printf("param %i %s",nr,c);
//reval = "aa"+expand_string(nr);

//text = strcat(alpha,text);
result = strpprintf(0,"end success ");
strg = result;
RETURN_STR(result);
}


/* {{{ php_man2html_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_man2html_init_globals(zend_man2html_globals *man2html_globals)
{
	man2html_globals->global_value = 0;
	man2html_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(man2html)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(man2html)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(man2html)
{
#if defined(COMPILE_DL_MAN2HTML) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(man2html)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(man2html)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "man2html support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ man2html_functions[]
 *
 * Every user visible function must have an entry in man2html_functions[].
 */
const zend_function_entry man2html_functions[] = {
	PHP_FE(confirm_man2html_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(mantest, NULL)	//my first test
	PHP_FE(man_main,NULL)
        PHP_FE(man_scan_troff, NULL)
        PHP_FE(man_scan_escape,NULL)
        PHP_FE(man_scan_request,NULL)
	PHP_FE_END	/* Must be the last line in man2html_functions[] */


};
/* }}} */

/* {{{ man2html_module_entry
 */
zend_module_entry man2html_module_entry = {
	STANDARD_MODULE_HEADER,
	"man2html",
	man2html_functions,
	PHP_MINIT(man2html),
	PHP_MSHUTDOWN(man2html),
	PHP_RINIT(man2html),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(man2html),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(man2html),
	PHP_MAN2HTML_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */


#ifdef COMPILE_DL_MAN2HTML
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(man2html)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
