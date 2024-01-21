#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>

// включение дополнительной печати для проверки для проверки обработки узлов AST в режиме отладки
#ifndef NDEBUG
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

enum CXChildVisitResult visitorCallback(CXCursor cursor, CXCursor parent, CXClientData clientData);
void printFunctionCall(FILE* file, const char* parentNode, CXCursor cursor);

// Структура для промежуточного хранения данных между вызовами функции visitorCallback для обработки узлов AST
typedef struct {
    FILE* graphvizFile; // Файл для записи Graphviz
    CXString parentCursorKind; // Идентификатор родительского узла
    const char* parentNode; //  Имя родительского узла
} StructDateSave;

// Функция для занесения найденных вызовов функций в файл для Graphviz
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
    enum CXCursorKind kind = clang_getCursorKind(cursor); // получение типа текущего курсора при обработке узлов AST
    CXString kindName = clang_getCursorKindSpelling(kind); // получение имени типа текущего курсора при обработке узлов AST
    CXString cursorKind = clang_getCursorSpelling(cursor); // получение имени текущего курсора при обработке узлов AST 
    if (kind == CXCursor_FunctionDecl){ // CXCursor_FunctionDecl - текущий курсор функция
        DBG("Cursor Type -> %s\t-Function!!!\tFunction name -> %s\n", clang_getCString(kindName), clang_getCString(cursorKind));

        // Очистка памяти для хранения имени идентификатора родительского узла в структуре промежуточного хранения данных при обработке узлов AST
        if (visitorData->parentNode != NULL){  // Если очистка требуется
            clang_disposeString(visitorData->parentCursorKind); // очищаем память
        }

        // Если очистка не требуется 
        visitorData->parentCursorKind = cursorKind; // Идентифицируем текущий курсор как родительский узел
        visitorData->parentNode = clang_getCString(visitorData->parentCursorKind); // Определяем имя родительского узла
        
    }
    else DBG("Cursor Type -> %s\n", clang_getCString(kindName));
    clang_disposeString(kindName); // очищаем память, выделенную под хранение имени типа текущего курсора
 
    // Проверка, является ли текущий курсор вызовом функции
    if (kind == CXCursor_CallExpr ) { // Если текущий курсор вызов функции (добавление || kind == CXCursor_DeclRefExpr скорее всего лишнее, стоит проанализировать и подумать какие типы курсоров еще можно добавить, но для этого по хорошему нужн полный вывод AST)
        printFunctionCall(visitorData->graphvizFile, visitorData->parentNode, cursor); // Вызываем функцию для занесения найденного вызова функции в файл для Graphviz     
    }
    
    // Продолжаем рекурсивный обход узлов AST
    return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
    
    // Получение имени основного исходного файла и аргументов компилятора из командной строки
    const char* mainFile = argv[1];
    const int clangArgsCount = (argc - 2) > 0 ? (argc - 2) : 0;
    const char* const* clangArgs = (clangArgsCount > 0) ? (const char* const*)(argv + 2) : NULL;

    // Создание индекса Clang
    CXIndex index = clang_createIndex(0, 0);

    //Проверка успешности создания index, если не успешно освобождаем ресурсы связанные с Clang
    if(!index) {
        fprintf(stderr, "error creating index\n");
        clang_disposeIndex(index);
        return 1;
    }

    // Парсинг исходного файла и создание дерева AST
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index,
        mainFile,
        clangArgs, clangArgsCount,
        NULL, 0,
        CXTranslationUnit_None);

    // Проверка успешности парсинга, если не успешно, освобождаем ресурсы, связанные с единицей парсинга TranslationUnit и индексом Clang
    if (!translationUnit) {
        fprintf(stderr, "Couldn't parse translationUnit\n");
        clang_disposeIndex(index);
        clang_disposeTranslationUnit(translationUnit);
        return 1;
    }

    // Создание файла для записи узлов AST для графа и дальшейней передачи его Graphviz, если файл не создался, свобождаем ресурсы, связанные с единицей парсинга TranslationUnit и индексом Clang
    FILE* graphvizFile = fopen("../output_graphviz/call_graph.dot", "w");
    if (!graphvizFile) {
        fprintf(stderr, "Couldn't create Graphviz file\n");
        clang_disposeIndex(index);
        clang_disposeTranslationUnit(translationUnit);
        return 1;
    }

    // Запись в файл заголовка необходимого для Graphviz
    fprintf(graphvizFile, "digraph CallGraph {\n");

    
    CXCursor rootCursor = clang_getTranslationUnitCursor(translationUnit); // Получение корневого курсора, с которго будет начинаться обработка узлов AST
    CXString nullCurcor = clang_getCursorSpelling(clang_getNullCursor()); // Получение "нулевого" курсора с использование clang_getNullCursor
    
    StructDateSave visitorData = { 
        graphvizFile, 
        {NULL,0},
        clang_getCString(nullCurcor)
        };

    clang_disposeString(nullCurcor); // очищение памяти связанной с получением имени нулевого курсора

    // Вызов функции обхода всех узлов AST начиная с корневого курсора с использованием функции обратного вызова для их обработки и использованием структуры данных для промежуточного хранения
    clang_visitChildren(rootCursor, visitorCallback, &visitorData);

    // После завершения обхода улов AST, запись в файл "завершающей части" для Graphviz
    fprintf(graphvizFile, "}\n");

    // Закрытие файла для Graphviz
    fclose(graphvizFile);

    // После окончания обхода и обработки AST, освобождение ресурсов, связанных с созданными единицей парсинга TranslationUnit и индексом Clang
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);

    //освобождение памяти, выделяемой в структуре промежуточного хранения данных для идентификатора родительского узла и оставшейся после окончания обхода и обработки узлов AST
    if (visitorData.parentNode != NULL){
        clang_disposeString(visitorData.parentCursorKind);
    }

    return 0;
}
