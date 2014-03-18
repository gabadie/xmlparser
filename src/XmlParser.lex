
%{
/* ----------------------------------------------------------------------------- C/C++ prefix */

#include "XmlParserInput.hpp"
#include "XmlDocument.hpp"
#include "XmlParser.hpp"


#ifdef FLEXDEBUG
#define dbg printf("[%s:%d] \"%s\"\n",__FILE__,__LINE__,xmltext)
#else
#define dbg ((void)0)
#endif


static char* buffer;

// supprime les espaces en début et fin de chaine
static char* supprimeEspaces(char *s) {
  char *buf;
  int i,lastnonspace;
  for (buf=s;isspace(*buf);buf++) ;
  for (i=0,lastnonspace=0;buf[i];i++) {
     if (!isspace(buf[i]))
        lastnonspace = i;
  }
  buf[lastnonspace+1] = '\0';
  return buf;
}

// enlève les " au début et à la fin
static char * enleveGuillemets(char *s) {
   s++;
   s[strlen(s)-1] = '\0';
   return s;
}


%}
/* ----------------------------------------------------------------------------- Flex configuration */
/* we don't have yywrap() */
%option 8bit noyywrap


/* ----------------------------------------------------------------------------- Terminals' definitions */

nl      (\r\n|\r|\n)
esp     [ \t\r\n]+
inf     {esp}?"<"
sup     ">"{nl}?
debutnom    [A-Za-z\200-\377_]
caracnom    [A-Za-z\200-\377_0-9.-]
esc     "&#"[0-9]+";"|"&#x"[0-9a-fA-F]+";"
nom     {debutnom}{caracnom}*
colon       ":"
data        ([^<\n&]|\n[^<&]|\n{esc}|{esc})+
pcdata      [^<]+
comment     {inf}"!--"([^-]|"-"[^-])*"--"{sup}
chaine      \"([^"&]|{esc})*\"|\'([^'&]|{esc})*\'
doctype     {esp}?"<!DOCTYPE"{esp}
infspecial {esp}?"<?"
supspecial "?>"{nl}?
cdata      "<![CDATA["
endcdata   "]]>"

/*
 * Le mode CONTENU est utilisé entre les balises ouvrantes et fermantes
 * Le mode INITIAL est utilisé en dehors de la balise racine ou entre les < >
 * Le mode CDATASECTION est utilisé dans les balises spéciales CDATA, le passage dans ce mode permet de ne rien parser
 */

%s CONTENU
%s CDATASECTION

/* ----------------------------------------------------------------------------- Terminals' rules */
%%

<INITIAL>{esp}      {/* skip */}
<INITIAL>{doctype} {dbg; return DOCTYPE;}
<INITIAL>"/"        {dbg; return SLASH;}
<INITIAL>"="        {dbg; return EGAL;}
<INITIAL>{sup}  {dbg; BEGIN(CONTENU); return SUP;}
<INITIAL>{supspecial}   { dbg; return SUPSPECIAL;}
<INITIAL>{nom}      {dbg; yylval.s = strdup(yytext); return NOM;}
<INITIAL>{colon}  {dbg; return COLON;}
<INITIAL>{chaine}   {dbg; yylval.s = strdup(enleveGuillemets(yytext)); return VALEUR;}
<CONTENU>{cdata}           {dbg; BEGIN(CDATASECTION); buffer = (char*) malloc(1); buffer[0]='\0'; return CDATABEGIN;  }
<CDATASECTION>{endcdata}     {dbg; BEGIN(CONTENU); yylval.s = strdup(buffer); free(buffer); return CDATAEND; }
<CDATASECTION>.              {dbg; char * tmp = strdup(buffer); free(buffer); buffer = (char*) malloc(strlen(tmp)+2); strcpy(buffer,tmp); strcat(buffer,yytext); free(tmp); }

<CONTENU,INITIAL>{infspecial}  {dbg; BEGIN(INITIAL); return INFSPECIAL;}
<CONTENU,INITIAL>{inf}         {dbg; BEGIN(INITIAL); return INF;}
<CONTENU,INITIAL>{comment}      {dbg; yylval.s = strdup(yytext); return COMMENT;}

<CONTENU>{pcdata}       {dbg; yylval.s = strdup(supprimeEspaces(yytext)); return DONNEES;}

. {
    std::cerr << "SCANNER ";
    yyerror("");
    exit(1);
}
