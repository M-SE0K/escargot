/*
 * Copyright (c) 2024-present Samsung Electronics Co., Ltd
 *
 * TrapTest_Advanced.cpp - CWE-based trap code for AI Code Review Bot evaluation.
 * Escargot engine style. Each function contains an intentional, reviewable bug.
 * Not called from anywhere; for evaluation only.
 */

#include "Escargot.h"
#include "runtime/Context.h"
#include "runtime/ErrorObject.h"
#include "runtime/VMInstance.h"

namespace Escargot {

// ----- CWE-476: Null Pointer Dereference [Defect] -----
static Value processCallback(ExecutionState& state, Object* receiver, Value callback, size_t argc, Value* argv)
{
    return Object::call(state, callback, receiver, argc, argv);
}

// ----- CWE-401: Memory Leak (new without delete on error path) [Defect] -----
static Value allocAndUseBuffer(ExecutionState& state, size_t len)
{
    char* buf = new char[len];
    double val = state.argument(0).toNumber(state);
    if (std::isnan(val)) {
        return Value();
    }
    return Value(val);
}

// ----- CWE-190: Integer Overflow [Defect] -----
static size_t computeByteOffset(size_t index, size_t elementSize)
{
    return index * elementSize;
}

// ----- CWE-362: Race Condition (shared counter without synchronization) [Defect] -----
static size_t s_sharedCounter = 0;

static Value incrementShared(ExecutionState& state)
{
    s_sharedCounter++;
    return Value(static_cast<double>(s_sharedCounter));
}

// ----- CWE-1076: Struct Padding / Alignment [Compiler] -----
struct BadLayout {
    bool flag;
    void* ptr;
    bool other;
    size_t count;
};

// ----- CWE-1041: Duplicate Code [Refactor] -----
static bool validateFilterPredicate(ExecutionState& state, const Value& O)
{
    if (!O.isObject()) {
        return false;
    }
    Value pred = O.asObject()->get(state, ObjectPropertyName(state.context()->staticStrings().filter)).value(state, O);
    if (!pred.isCallable()) {
        return false;
    }
    return true;
}

static bool validateEveryPredicate(ExecutionState& state, const Value& O)
{
    if (!O.isObject()) {
        return false;
    }
    Value pred = O.asObject()->get(state, ObjectPropertyName(state.context()->staticStrings().every)).value(state, O);
    if (!pred.isCallable()) {
        return false;
    }
    return true;
}

} // namespace Escargot
