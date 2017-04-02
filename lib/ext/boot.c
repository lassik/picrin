#include "picrin.h"
#include "picrin/extra.h"

static const char boot_rom[][80] = {
"(core#define-macro call-with-current-environment (core#lambda (form env) (list (",
"cadr form) env))) (core#define here (call-with-current-environment (core#lambda ",
"(env) env))) (core#define the (core#lambda (var) (make-identifier var here))) (c",
"ore#define the-builtin-define (the (core#quote core#define))) (core#define the-b",
"uiltin-lambda (the (core#quote core#lambda))) (core#define the-builtin-begin (th",
"e (core#quote core#begin))) (core#define the-builtin-quote (the (core#quote core",
"#quote))) (core#define the-builtin-set! (the (core#quote core#set!))) (core#defi",
"ne the-builtin-if (the (core#quote core#if))) (core#define the-builtin-define-ma",
"cro (the (core#quote core#define-macro))) (core#define the-define (the (core#quo",
"te define))) (core#define the-lambda (the (core#quote lambda))) (core#define the",
"-begin (the (core#quote begin))) (core#define the-quote (the (core#quote quote))",
") (core#define the-set! (the (core#quote set!))) (core#define the-if (the (core#",
"quote if))) (core#define the-define-macro (the (core#quote define-macro))) (core",
"#define-macro quote (core#lambda (form env) (core#if (= (length form) 2) (list t",
"he-builtin-quote (cadr form)) (error \"illegal quote form\" form)))) (core#define-",
"macro if (core#lambda (form env) ((core#lambda (len) (core#if (= len 4) (cons th",
"e-builtin-if (cdr form)) (core#if (= len 3) (list the-builtin-if (list-ref form ",
"1) (list-ref form 2) #undefined) (error \"illegal if form\" form)))) (length form)",
"))) (core#define-macro begin (core#lambda (form env) ((core#lambda (len) (if (= ",
"len 1) #undefined (if (= len 2) (cadr form) (if (= len 3) (cons the-builtin-begi",
"n (cdr form)) (list the-builtin-begin (cadr form) (cons the-begin (cddr form))))",
"))) (length form)))) (core#define-macro set! (core#lambda (form env) (if (= (len",
"gth form) 3) (if (identifier? (cadr form)) (cons the-builtin-set! (cdr form)) (e",
"rror \"illegal set! form\" form)) (error \"illegal set! form\" form)))) (core#define",
" check-formal (core#lambda (formal) (if (null? formal) #t (if (identifier? forma",
"l) #t (if (pair? formal) (if (identifier? (car formal)) (check-formal (cdr forma",
"l)) #f) #f))))) (core#define-macro lambda (core#lambda (form env) (if (= (length",
" form) 1) (error \"illegal lambda form\" form) (if (check-formal (cadr form)) (lis",
"t the-builtin-lambda (cadr form) (cons the-begin (cddr form))) (error \"illegal l",
"ambda form\" form))))) (core#define-macro define (lambda (form env) ((lambda (len",
") (if (= len 1) (error \"illegal define form\" form) (if (identifier? (cadr form))",
" (if (= len 3) (cons the-builtin-define (cdr form)) (error \"illegal define form\"",
" form)) (if (pair? (cadr form)) (list the-define (car (cadr form)) (cons the-lam",
"bda (cons (cdr (cadr form)) (cddr form)))) (error \"define: binding to non-varaib",
"le object\" form))))) (length form)))) (core#define-macro define-macro (lambda (f",
"orm env) (if (= (length form) 3) (if (identifier? (cadr form)) (cons the-builtin",
"-define-macro (cdr form)) (error \"define-macro: binding to non-variable object\" ",
"form)) (error \"illegal define-macro form\" form)))) (define-macro syntax-error (l",
"ambda (form _) (apply error (cdr form)))) (define-macro define-auxiliary-syntax ",
"(lambda (form _) (define message (string-append \"invalid use of auxiliary syntax",
": '\" (symbol->string (cadr form)) \"'\")) (list the-define-macro (cadr form) (list",
" the-lambda '_ (list (the 'error) message))))) (define-auxiliary-syntax else) (d",
"efine-auxiliary-syntax =>) (define-auxiliary-syntax unquote) (define-auxiliary-s",
"yntax unquote-splicing) (define-auxiliary-syntax syntax-unquote) (define-auxilia",
"ry-syntax syntax-unquote-splicing) (define-macro let (lambda (form env) (if (ide",
"ntifier? (cadr form)) (list (list the-lambda '() (list the-define (cadr form) (c",
"ons the-lambda (cons (map car (car (cddr form))) (cdr (cddr form))))) (cons (cad",
"r form) (map cadr (car (cddr form)))))) (cons (cons the-lambda (cons (map car (c",
"adr form)) (cddr form))) (map cadr (cadr form)))))) (define-macro and (lambda (f",
"orm env) (if (null? (cdr form)) #t (if (null? (cddr form)) (cadr form) (list the",
"-if (cadr form) (cons (the 'and) (cddr form)) #f))))) (define-macro or (lambda (",
"form env) (if (null? (cdr form)) #f (let ((tmp (make-identifier 'it env))) (list",
" (the 'let) (list (list tmp (cadr form))) (list the-if tmp tmp (cons (the 'or) (",
"cddr form)))))))) (define-macro cond (lambda (form env) (let ((clauses (cdr form",
"))) (if (null? clauses) #undefined (let ((clause (car clauses))) (if (and (ident",
"ifier? (car clause)) (identifier=? (the 'else) (make-identifier (car clause) env",
"))) (cons the-begin (cdr clause)) (if (null? (cdr clause)) (let ((tmp (make-iden",
"tifier 'tmp here))) (list (the 'let) (list (list tmp (car clause))) (list the-if",
" tmp tmp (cons (the 'cond) (cdr clauses))))) (if (and (identifier? (cadr clause)",
") (identifier=? (the '=>) (make-identifier (cadr clause) env))) (let ((tmp (make",
"-identifier 'tmp here))) (list (the 'let) (list (list tmp (car clause))) (list t",
"he-if tmp (list (car (cddr clause)) tmp) (cons (the 'cond) (cdr clauses))))) (li",
"st the-if (car clause) (cons the-begin (cdr clause)) (cons (the 'cond) (cdr clau",
"ses))))))))))) (define-macro quasiquote (lambda (form env) (define (quasiquote? ",
"form) (and (pair? form) (identifier? (car form)) (identifier=? (the 'quasiquote)",
" (make-identifier (car form) env)))) (define (unquote? form) (and (pair? form) (",
"identifier? (car form)) (identifier=? (the 'unquote) (make-identifier (car form)",
" env)))) (define (unquote-splicing? form) (and (pair? form) (pair? (car form)) (",
"identifier? (caar form)) (identifier=? (the 'unquote-splicing) (make-identifier ",
"(caar form) env)))) (define (qq depth expr) (cond ((unquote? expr) (if (= depth ",
"1) (car (cdr expr)) (list (the 'list) (list (the 'quote) (the 'unquote)) (qq (- ",
"depth 1) (car (cdr expr)))))) ((unquote-splicing? expr) (if (= depth 1) (list (t",
"he 'append) (car (cdr (car expr))) (qq depth (cdr expr))) (list (the 'cons) (lis",
"t (the 'list) (list (the 'quote) (the 'unquote-splicing)) (qq (- depth 1) (car (",
"cdr (car expr))))) (qq depth (cdr expr))))) ((quasiquote? expr) (list (the 'list",
") (list (the 'quote) (the 'quasiquote)) (qq (+ depth 1) (car (cdr expr))))) ((pa",
"ir? expr) (list (the 'cons) (qq depth (car expr)) (qq depth (cdr expr)))) ((vect",
"or? expr) (list (the 'list->vector) (qq depth (vector->list expr)))) (else (list",
" (the 'quote) expr)))) (let ((x (cadr form))) (qq 1 x)))) (define-macro let* (la",
"mbda (form env) (let ((bindings (car (cdr form))) (body (cdr (cdr form)))) (if (",
"null? bindings) `(,(the 'let) () ,@body) `(,(the 'let) ((,(car (car bindings)) ,",
"@(cdr (car bindings)))) (,(the 'let*) (,@(cdr bindings)) ,@body)))))) (define-ma",
"cro letrec (lambda (form env) `(,(the 'letrec*) ,@(cdr form)))) (define-macro le",
"trec* (lambda (form env) (let ((bindings (car (cdr form))) (body (cdr (cdr form)",
"))) (let ((variables (map (lambda (v) `(,v #f)) (map car bindings))) (initials (",
"map (lambda (v) `(,(the 'set!) ,@v)) bindings))) `(,(the 'let) (,@variables) ,@i",
"nitials ,@body))))) (define-macro let-values (lambda (form env) `(,(the 'let*-va",
"lues) ,@(cdr form)))) (define-macro let*-values (lambda (form env) (let ((formal",
" (car (cdr form))) (body (cdr (cdr form)))) (if (null? formal) `(,(the 'let) () ",
",@body) `(,(the 'call-with-values) (,the-lambda () ,@(cdr (car formal))) (,(the ",
"'lambda) (,@(car (car formal))) (,(the 'let*-values) (,@(cdr formal)) ,@body))))",
"))) (define-macro define-values (lambda (form env) (let ((formal (car (cdr form)",
")) (body (cdr (cdr form)))) (let ((arguments (make-identifier 'arguments here)))",
" `(,the-begin ,@(let loop ((formal formal)) (if (pair? formal) `((,the-define ,(",
"car formal) #undefined) ,@(loop (cdr formal))) (if (identifier? formal) `((,the-",
"define ,formal #undefined)) '()))) (,(the 'call-with-values) (,the-lambda () ,@b",
"ody) (,the-lambda ,arguments ,@(let loop ((formal formal) (args arguments)) (if ",
"(pair? formal) `((,the-set! ,(car formal) (,(the 'car) ,args)) ,@(loop (cdr form",
"al) `(,(the 'cdr) ,args))) (if (identifier? formal) `((,the-set! ,formal ,args))",
" '())))))))))) (define-macro do (lambda (form env) (let ((bindings (car (cdr for",
"m))) (test (car (car (cdr (cdr form))))) (cleanup (cdr (car (cdr (cdr form))))) ",
"(body (cdr (cdr (cdr form))))) (let ((loop (make-identifier 'loop here))) `(,(th",
"e 'let) ,loop ,(map (lambda (x) `(,(car x) ,(cadr x))) bindings) (,the-if ,test ",
"(,the-begin ,@cleanup) (,the-begin ,@body (,loop ,@(map (lambda (x) (if (null? (",
"cdr (cdr x))) (car x) (car (cdr (cdr x))))) bindings))))))))) (define-macro when",
" (lambda (form env) (let ((test (car (cdr form))) (body (cdr (cdr form)))) `(,th",
"e-if ,test (,the-begin ,@body) #undefined)))) (define-macro unless (lambda (form",
" env) (let ((test (car (cdr form))) (body (cdr (cdr form)))) `(,the-if ,test #un",
"defined (,the-begin ,@body))))) (define-macro case (lambda (form env) (let ((key",
" (car (cdr form))) (clauses (cdr (cdr form)))) (let ((the-key (make-identifier '",
"key here))) `(,(the 'let) ((,the-key ,key)) ,(let loop ((clauses clauses)) (if (",
"null? clauses) #undefined (let ((clause (car clauses))) `(,the-if ,(if (and (ide",
"ntifier? (car clause)) (identifier=? (the 'else) (make-identifier (car clause) e",
"nv))) #t `(,(the 'or) ,@(map (lambda (x) `(,(the 'eqv?) ,the-key (,the-quote ,x)",
")) (car clause)))) ,(if (and (identifier? (cadr clause)) (identifier=? (the '=>)",
" (make-identifier (cadr clause) env))) `(,(car (cdr (cdr clause))) ,the-key) `(,",
"the-begin ,@(cdr clause))) ,(loop (cdr clauses))))))))))) (define-macro paramete",
"rize (lambda (form env) (let ((formal (car (cdr form))) (body (cdr (cdr form))))",
" `(,(the 'with-dynamic-environment) (,(the 'list) ,@(map (lambda (x) `(,(the 'co",
"ns) ,(car x) ,(cadr x))) formal)) (,the-lambda () ,@body))))) (define-macro synt",
"ax-quote (lambda (form env) (let ((renames '())) (letrec ((rename (lambda (var) ",
"(let ((x (assq var renames))) (if x (cadr x) (begin (set! renames `((,var ,(make",
"-identifier var env) (,(the 'make-identifier) ',var ',env)) unquote renames)) (r",
"ename var)))))) (walk (lambda (f form) (cond ((identifier? form) (f form)) ((pai",
"r? form) `(,(the 'cons) (walk f (car form)) (walk f (cdr form)))) ((vector? form",
") `(,(the 'list->vector) (walk f (vector->list form)))) (else `(,(the 'quote) ,f",
"orm)))))) (let ((form (walk rename (cadr form)))) `(,(the 'let) ,(map cdr rename",
"s) ,form)))))) (define-macro syntax-quasiquote (lambda (form env) (let ((renames",
" '())) (letrec ((rename (lambda (var) (let ((x (assq var renames))) (if x (cadr ",
"x) (begin (set! renames `((,var ,(make-identifier var env) (,(the 'make-identifi",
"er) ',var ',env)) unquote renames)) (rename var))))))) (define (syntax-quasiquot",
"e? form) (and (pair? form) (identifier? (car form)) (identifier=? (the 'syntax-q",
"uasiquote) (make-identifier (car form) env)))) (define (syntax-unquote? form) (a",
"nd (pair? form) (identifier? (car form)) (identifier=? (the 'syntax-unquote) (ma",
"ke-identifier (car form) env)))) (define (syntax-unquote-splicing? form) (and (p",
"air? form) (pair? (car form)) (identifier? (caar form)) (identifier=? (the 'synt",
"ax-unquote-splicing) (make-identifier (caar form) env)))) (define (qq depth expr",
") (cond ((syntax-unquote? expr) (if (= depth 1) (car (cdr expr)) (list (the 'lis",
"t) (list (the 'quote) (the 'syntax-unquote)) (qq (- depth 1) (car (cdr expr)))))",
") ((syntax-unquote-splicing? expr) (if (= depth 1) (list (the 'append) (car (cdr",
" (car expr))) (qq depth (cdr expr))) (list (the 'cons) (list (the 'list) (list (",
"the 'quote) (the 'syntax-unquote-splicing)) (qq (- depth 1) (car (cdr (car expr)",
")))) (qq depth (cdr expr))))) ((syntax-quasiquote? expr) (list (the 'list) (list",
" (the 'quote) (the 'quasiquote)) (qq (+ depth 1) (car (cdr expr))))) ((pair? exp",
"r) (list (the 'cons) (qq depth (car expr)) (qq depth (cdr expr)))) ((vector? exp",
"r) (list (the 'list->vector) (qq depth (vector->list expr)))) ((identifier? expr",
") (rename expr)) (else (list (the 'quote) expr)))) (let ((body (qq 1 (cadr form)",
"))) `(,(the 'let) ,(map cdr renames) ,body)))))) (define (transformer f) (lambda",
" (form env) (let ((ephemeron1 (make-ephemeron-table)) (ephemeron2 (make-ephemero",
"n-table))) (letrec ((wrap (lambda (var1) (let ((var2 (ephemeron1 var1))) (if var",
"2 (cdr var2) (let ((var2 (make-identifier var1 env))) (ephemeron1 var1 var2) (ep",
"hemeron2 var2 var1) var2))))) (unwrap (lambda (var2) (let ((var1 (ephemeron2 var",
"2))) (if var1 (cdr var1) var2)))) (walk (lambda (f form) (cond ((identifier? for",
"m) (f form)) ((pair? form) (cons (walk f (car form)) (walk f (cdr form)))) ((vec",
"tor? form) (list->vector (walk f (vector->list form)))) (else form))))) (let ((f",
"orm (cdr form))) (walk unwrap (apply f (walk wrap form)))))))) (define-macro def",
"ine-syntax (lambda (form env) (let ((formal (car (cdr form))) (body (cdr (cdr fo",
"rm)))) (if (pair? formal) `(,(the 'define-syntax) ,(car formal) (,the-lambda ,(c",
"dr formal) ,@body)) `(,the-define-macro ,formal (,(the 'transformer) (,the-begin",
" ,@body))))))) (define-macro letrec-syntax (lambda (form env) (let ((formal (car",
" (cdr form))) (body (cdr (cdr form)))) `(let () ,@(map (lambda (x) `(,(the 'defi",
"ne-syntax) ,(car x) ,(cadr x))) formal) ,@body)))) (define-macro let-syntax (lam",
"bda (form env) `(,(the 'letrec-syntax) ,@(cdr form)))) (define (mangle name) (wh",
"en (null? name) (error \"library name should be a list of at least one symbols\" n",
"ame)) (define (->string n) (cond ((symbol? n) (let ((str (symbol->string n))) (s",
"tring-for-each (lambda (c) (when (or (char=? c #\\.) (char=? c #\\:)) (error \"elem",
"ents of library name may not contain '.' or ':'\" n))) str) str)) ((and (number? ",
"n) (exact? n) (<= 0 n)) (number->string n)) (else (error \"symbol or non-negative",
" integer is required\" n)))) (define (join strs delim) (let loop ((res (car strs)",
") (strs (cdr strs))) (if (null? strs) res (loop (string-append res delim (car st",
"rs)) (cdr strs))))) (if (symbol? name) name (string->symbol (join (map ->string ",
"name) \".\")))) (define current-library (make-parameter '(picrin base) mangle)) (d",
"efine *libraries* (make-dictionary)) (define (find-library name) (dictionary-has",
"? *libraries* (mangle name))) (define (make-library name) (let ((name (mangle na",
"me))) (let ((env (make-environment (string->symbol (string-append (symbol->strin",
"g name) \":\")))) (exports (make-dictionary))) (set-identifier! 'define-library 'd",
"efine-library env) (set-identifier! 'import 'import env) (set-identifier! 'expor",
"t 'export env) (set-identifier! 'cond-expand 'cond-expand env) (dictionary-set! ",
"*libraries* name `(,env unquote exports))))) (define (library-environment name) ",
"(car (dictionary-ref *libraries* (mangle name)))) (define (library-exports name)",
" (cdr (dictionary-ref *libraries* (mangle name)))) (define (library-import name ",
"sym alias) (let ((uid (dictionary-ref (library-exports name) sym))) (let ((env (",
"library-environment (current-library)))) (set-identifier! alias uid env)))) (def",
"ine (library-export sym alias) (let ((env (library-environment (current-library)",
")) (exports (library-exports (current-library)))) (dictionary-set! exports alias",
" (find-identifier sym env)))) (define-macro define-library (lambda (form _) (let",
" ((name (cadr form)) (body (cddr form))) (or (find-library name) (make-library n",
"ame)) (parameterize ((current-library name)) (for-each (lambda (expr) (eval expr",
" name)) body))))) (define-macro cond-expand (lambda (form _) (letrec ((test (lam",
"bda (form) (or (eq? form 'else) (and (symbol? form) (memq form (features))) (and",
" (pair? form) (case (car form) ((library) (find-library (cadr form))) ((not) (no",
"t (test (cadr form)))) ((and) (let loop ((form (cdr form))) (or (null? form) (an",
"d (test (car form)) (loop (cdr form)))))) ((or) (let loop ((form (cdr form))) (a",
"nd (pair? form) (or (test (car form)) (loop (cdr form)))))) (else #f))))))) (let",
" loop ((clauses (cdr form))) (if (null? clauses) #undefined (if (test (caar clau",
"ses)) `(,the-begin ,@(cdar clauses)) (loop (cdr clauses)))))))) (define-macro im",
"port (lambda (form _) (let ((caddr (lambda (x) (car (cdr (cdr x))))) (prefix (la",
"mbda (prefix symbol) (string->symbol (string-append (symbol->string prefix) (sym",
"bol->string symbol))))) (getlib (lambda (name) (if (find-library name) name (err",
"or \"library not found\" name))))) (letrec ((extract (lambda (spec) (case (car spe",
"c) ((only rename prefix except) (extract (cadr spec))) (else (getlib spec))))) (",
"collect (lambda (spec) (case (car spec) ((only) (let ((alist (collect (cadr spec",
")))) (map (lambda (var) (assq var alist)) (cddr spec)))) ((rename) (let ((alist ",
"(collect (cadr spec))) (renames (map (lambda (x) `(,(car x) unquote (cadr x))) (",
"cddr spec)))) (map (lambda (s) (or (assq (car s) renames) s)) alist))) ((prefix)",
" (let ((alist (collect (cadr spec)))) (map (lambda (s) (cons (prefix (caddr spec",
") (car s)) (cdr s))) alist))) ((except) (let ((alist (collect (cadr spec)))) (le",
"t loop ((alist alist)) (if (null? alist) '() (if (memq (caar alist) (cddr spec))",
" (loop (cdr alist)) (cons (car alist) (loop (cdr alist)))))))) (else (dictionary",
"-map (lambda (x) (cons x x)) (library-exports (getlib spec)))))))) (letrec ((imp",
"ort (lambda (spec) (let ((lib (extract spec)) (alist (collect spec))) (for-each ",
"(lambda (slot) (library-import lib (cdr slot) (car slot))) alist))))) (for-each ",
"import (cdr form))))))) (define-macro export (lambda (form _) (letrec ((collect ",
"(lambda (spec) (cond ((symbol? spec) `(,spec unquote spec)) ((and (list? spec) (",
"= (length spec) 3) (eq? (car spec) 'rename)) `(,(list-ref spec 1) unquote (list-",
"ref spec 2))) (else (error \"malformed export\"))))) (export (lambda (spec) (let (",
"(slot (collect spec))) (library-export (car slot) (cdr slot)))))) (for-each expo",
"rt (cdr form))))) (let () (make-library '(picrin base)) (set-car! (dictionary-re",
"f *libraries* (mangle '(picrin base))) default-environment) (let ((export-keywor",
"ds (lambda (keywords) (let ((env (library-environment '(picrin base))) (exports ",
"(library-exports '(picrin base)))) (for-each (lambda (keyword) (dictionary-set! ",
"exports keyword keyword)) keywords))))) (export-keywords '(define lambda quote s",
"et! if begin define-macro let let* letrec letrec* let-values let*-values define-",
"values quasiquote unquote unquote-splicing and or cond case else => do when unle",
"ss parameterize define-syntax syntax-quote syntax-unquote syntax-quasiquote synt",
"ax-unquote-splicing let-syntax letrec-syntax syntax-error)) (export-keywords '(f",
"eatures eq? eqv? equal? not boolean? boolean=? pair? cons car cdr null? set-car!",
" set-cdr! caar cadr cdar cddr list? make-list list length append reverse list-ta",
"il list-ref list-set! list-copy map for-each memq memv member assq assv assoc cu",
"rrent-input-port current-output-port current-error-port port? input-port? output",
"-port? port-open? close-port eof-object? eof-object read-u8 peek-u8 read-bytevec",
"tor! write-u8 write-bytevector flush-output-port open-input-bytevector open-outp",
"ut-bytevector get-output-bytevector number? exact? inexact? inexact exact = < > ",
"<= >= + - * / number->string string->number procedure? apply symbol? symbol=? sy",
"mbol->string string->symbol make-identifier identifier? identifier=? identifier-",
"base identifier-environment vector? vector make-vector vector-length vector-ref ",
"vector-set! vector-copy! vector-copy vector-append vector-fill! vector-map vecto",
"r-for-each list->vector vector->list string->vector vector->string bytevector? b",
"ytevector make-bytevector bytevector-length bytevector-u8-ref bytevector-u8-set!",
" bytevector-copy! bytevector-copy bytevector-append bytevector->list list->bytev",
"ector call-with-current-continuation call/cc values call-with-values char? char-",
">integer integer->char char=? char<? char>? char<=? char>=? current-exception-ha",
"ndlers with-exception-handler raise raise-continuable error error-object? error-",
"object-message error-object-irritants error-object-type string? string make-stri",
"ng string-length string-ref string-set! string-copy string-copy! string-fill! st",
"ring-append string-map string-for-each list->string string->list string=? string",
"<? string>? string<=? string>=? make-parameter with-dynamic-environment read mak",
"e-dictionary dictionary? dictionary dictionary-has? dictionary-ref dictionary-se",
"t! dictionary-delete! dictionary-size dictionary-map dictionary-for-each diction",
"ary->alist alist->dictionary dictionary->plist plist->dictionary make-record rec",
"ord? record-type record-datum default-environment make-environment find-identifi",
"er set-identifier! eval make-ephemeron-table write write-simple write-shared dis",
"play)) (export-keywords '(find-library make-library current-library))) (set! eva",
"l (let ((e eval)) (lambda (expr . lib) (let ((lib (if (null? lib) (current-libra",
"ry) (car lib)))) (e expr (library-environment lib)))))) (make-library '(picrin u",
"ser)) (current-library '(picrin user))) ",
};

void
pic_boot(pic_state *pic)
{
  pic_load_cstr(pic, &boot_rom[0][0]);
}
