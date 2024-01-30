#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

enum CXChildVisitResult visitorCallback(CXCursor cursor, CXCursor parent, CXClientData clientData);
void printFunctionCall(FILE* file, const char* parentNode, CXCursor cursor);

typedef struct {
    FILE* graphvizFile; 
    CXString parentCursorKind; 
    const char* parentNode; 
} StructDateSave;

void printFunctionCall(FILE* file, const char* parentNode, CXCursor cursor) {
    DBG("-----------------------------------------------------------------------------------------------------------------------------\n");
    DBG("function call 'printFunctionCall'\n");
    CXString childrenNode = clang_getCursorSpelling(cursor);
    DBG("cursorVisitor (childrenNode) -> '%s'\n",clang_getCString(childrenNode));
    DBG("parentNode -> %s\n",parentNode);
    fprintf(file, "  \"%s\" -> \"%s\"\n", parentNode, clang_getCString(childrenNode));
    DBG("-----------------------------------------------------------------------------------------------------------------------------\n");
    clang_disposeString(childrenNode);
}

// Функция обратного вызова для обработки каждого узла AST
enum CXChildVisitResult visitorCallback(CXCursor cursor, CXCursor visitor, CXClientData clientData) {

    (void) visitor; // прибил warning связанный с ненужным параметром CXCursor visitor

    StructDateSave* visitorData = (StructDateSave*)clientData; // Приведение указателя на данные клиента к типу StructDateSave

    CXSourceLocation location = clang_getCursorLocation(cursor);
    if( clang_Location_isFromMainFile(location) == 0 ){
        return CXChildVisit_Continue;
    }
    
    //Определение типа и имени родительского узла при обработке AST
    enum CXCursorKind kind = clang_getCursorKind(cursor); 
    CXString kindName = clang_getCursorKindSpelling(kind);  
    if (kind == CXCursor_FunctionDecl){ 
        CXString cursorKind = clang_getCursorSpelling(cursor);
        DBG("Cursor Type -> %s\t-Function!!!\tFunction name -> %s\n", clang_getCString(kindName), clang_getCString(cursorKind));

        if (visitorData->parentNode != NULL){  
            clang_disposeString(visitorData->parentCursorKind); 
        }

        visitorData->parentCursorKind = cursorKind;
        visitorData->parentNode = clang_getCString(visitorData->parentCursorKind); 
        
    }
    else DBG("Cursor Type -> %s\n", clang_getCString(kindName));
    clang_disposeString(kindName);
 
   
    if (kind == CXCursor_CallExpr ) { 
        printFunctionCall(visitorData->graphvizFile, visitorData->parentNode, cursor); // Вызываем функцию для занесения найденного вызова функции в файл для Graphviz     
    }
    
    // Продолжаем рекурсивный обход узлов AST
    return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
	
	if (argc < 2){
		fprintf(stderr, "Invalid args count\n");
		printf("Correct usage:\n");
		printf("./clang_graphviz <source_file_for_analize> [clang_arg1] [clang_arg2] [...]\n");
		return 1;
	}
	    
    const char* mainFile = argv[1];
    const int clangArgsCount = (argc - 2) > 0 ? (argc - 2) : 0;
    const char* const* clangArgs = (clangArgsCount > 0) ? (const char* const*)(argv + 2) : NULL;

    CXIndex index = clang_createIndex(0, 0);

    if(!index) {
        fprintf(stderr, "error creating index\n");
        clang_disposeIndex(index);
        return 1;
    }

    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index,
        mainFile,
        clangArgs, clangArgsCount,
        NULL, 0,
        CXTranslationUnit_None);

    if (!translationUnit) {
        fprintf(stderr, "Couldn't parse translationUnit\n");
        clang_disposeIndex(index);
        clang_disposeTranslationUnit(translationUnit);
        return 1;
    }

    FILE* graphvizFile = fopen("./call_graph.dot", "w");
    if (!graphvizFile) {
        fprintf(stderr, "Couldn't create Graphviz file\n");
        clang_disposeIndex(index);
        clang_disposeTranslationUnit(translationUnit);
        return 1;
    }

        fprintf(graphvizFile, "digraph CallGraph {\n"); // Запись заголовка необходимого для Graphviz

    
    CXCursor rootCursor = clang_getTranslationUnitCursor(translationUnit); 
    CXString nullCurcor = clang_getCursorSpelling(clang_getNullCursor());
    
    StructDateSave visitorData = { 
        graphvizFile, 
        {NULL,0},
        clang_getCString(nullCurcor)
        };

    clang_disposeString(nullCurcor); 

    clang_visitChildren(rootCursor, visitorCallback, &visitorData);

    
    fprintf(graphvizFile, "}\n"); // запись "завершающей части" для Graphviz

    fclose(graphvizFile);

    // освобождение ресурсов 
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);

    if (visitorData.parentNode != NULL){
        clang_disposeString(visitorData.parentCursorKind);
    }

    return 0;
}
