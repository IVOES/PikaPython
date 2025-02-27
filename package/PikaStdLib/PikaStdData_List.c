#include "PikaStdData_List.h"
#include "BaseObj.h"
#include "PikaObj.h"
#include "PikaStdData_Tuple.h"
#include "dataStrs.h"

void PikaStdData_List_append(PikaObj* self, Arg* arg) {
    objList_append(self, arg);
}

void PikaStdData_List_set(PikaObj* self, int i, Arg* arg) {
    return objList_set(self, i, arg);
}

void PikaStdData_List___setitem__(PikaObj* self, Arg* __key, Arg* __val) {
    PikaStdData_List_set(self, obj_getInt(self, "__key"),
                         obj_getArg(self, "__val"));
}

void PikaStdData_List___init__(PikaObj* self) {
    objList_init(self);
}

char* builtins_str(PikaObj* self, Arg* arg);
typedef struct {
    Arg* buf;
    int count;
} ListToStrContext;

int32_t listToStrEachHandle(PikaObj* self,
                            int itemIndex,
                            Arg* itemEach,
                            void* context) {
    ListToStrContext* ctx = (ListToStrContext*)context;

    char* item_str = builtins_str(self, itemEach);
    if (ctx->count != 0) {
        ctx->buf = arg_strAppend(ctx->buf, ", ");
    }
    if (arg_getType(itemEach) == ARG_TYPE_STRING) {
        ctx->buf = arg_strAppend(ctx->buf, "'");
    }
    ctx->buf = arg_strAppend(ctx->buf, item_str);
    if (arg_getType(itemEach) == ARG_TYPE_STRING) {
        ctx->buf = arg_strAppend(ctx->buf, "'");
    }

    ctx->count++;

    return 0;
}

char* PikaStdData_List___str__(PikaObj* self) {
    ListToStrContext context;
    context.buf = arg_newStr("[");
    context.count = 0;

    objList_forEach(self, listToStrEachHandle, &context);

    context.buf = arg_strAppend(context.buf, "]");
    obj_setStr(self, "_buf", arg_getStr(context.buf));
    arg_deinit(context.buf);
    return obj_getStr(self, "_buf");
}

void PikaStdData_List_reverse(PikaObj* self) {
    PikaList* list = obj_getPtr(self, "list");
    pikaList_reverse(list);
}

PikaObj* PikaStdData_List___add__(PikaObj* self, PikaObj* others) {
    PikaObj* res = newNormalObj(New_PikaStdData_List);
    PikaStdData_List___init__(res);
    PikaList* list_res = obj_getPtr(res, "list");
    PikaList* list1 = obj_getPtr(self, "list");
    PikaList* list2 = obj_getPtr(others, "list");
    for (size_t i = 0; i < pikaList_getSize(list1); i++) {
        Arg* arg = pikaList_getArg(list1, i);
        pikaList_append(list_res, arg);
    }
    for (size_t i = 0; i < pikaList_getSize(list2); i++) {
        Arg* arg = pikaList_getArg(list2, i);
        pikaList_append(list_res, arg);
    }
    return res;
}

void PikaStdData_List_insert(PikaObj* self, int i, Arg* arg) {
    PikaList* list = obj_getPtr(self, "list");
    if (PIKA_RES_OK != pikaList_insert(list, i, arg)) {
        obj_setErrorCode(self, 1);
        obj_setSysOut(self, "Error: index exceeded lengh of list.");
    }
}

Arg* PikaStdData_List_pop(PikaObj* self, PikaTuple* index) {
    int i = 0;
    PikaList* list = obj_getPtr(self, "list");
    if (pikaTuple_getSize(index) == 1) {
        if (pikaTuple_getType(index, 0) == ARG_TYPE_INT) {
            i = pikaTuple_getInt(index, 0);
        } else {
            obj_setErrorCode(self, 1);
            obj_setSysOut(self, "Error: index must be int.");
            return NULL;
        }
        return pikaList_pop_withIndex(list, i);
    } else {
        return pikaList_pop(list);
    }
}

void PikaStdData_List_remove(PikaObj* self, Arg* val) {
    PikaList* list = obj_getPtr(self, "list");
    pikaList_remove(list, val);
}
