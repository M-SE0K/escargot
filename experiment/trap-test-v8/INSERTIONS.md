# trap-test-v8 함정 코드 삽입 위치 (신규 builtin 전용)

v5·v6·v7과 **다른 파일**에만 삽입. 주석 없음.

---

## 1. BuiltinMath.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 369 | `builtinMathRound` | `div = (argc > 1) ? argv[1].toNumber(state) : 0.0`, `(void)(1.0 / div);` (0 나누기 가능) |

---

## 2. BuiltinFunction.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinFunctionConstructor` | 전역 `static size_t s_functionConstructCount = 0;`, 생성자 맨 앞에 `s_functionConstructCount++;` (동기화 없음) |

---

## 3. BuiltinAtomics.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinAtomicsLoad` | 전역 `static size_t s_atomicsLoadCount = 0;`, 함수 맨 앞에 `s_atomicsLoadCount++;` (동기화 없음) |

---

## 4. BuiltinWeakRef.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinWeakRefConstructor` | 전역 `static size_t s_weakRefConstructCount = 0;`, 생성자 맨 앞에 `s_weakRefConstructCount++;` (동기화 없음) |

---

## 5. BuiltinIntl.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 476 | `builtinIntlCollatorConstructor` | `optObj = options.isObject() ? options.asObject() : nullptr` 후, `(void)optObj->getPrototype(state);` (argc<2이면 options 미설정 → null 역참조) |

---

## 6. BuiltinGeneratorFunction.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinGeneratorFunction` | 전역 `static size_t s_generatorFunctionCallCount = 0;`, 함수 맨 앞에 `s_generatorFunctionCallCount++;` (동기화 없음) |

---

## 7. BuiltinAsyncFunction.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinAsyncFunction` | 전역 `static size_t s_asyncFunctionConstructCount = 0;`, 함수 맨 앞에 `s_asyncFunctionConstructCount++;` (동기화 없음) |

---

## 8. BuiltinAsyncFromSyncIterator.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 476 | `builtinAsyncFromSyncIteratorNext` | `oPtr = O.isObject() ? O.asObject() : nullptr` 후, `(void)oPtr->getPrototype(state);` (this가 비객체면 null 역참조) |

---

## 요약

| 파일 | CWE | 함수 |
|------|-----|------|
| BuiltinMath.cpp | 369 | builtinMathRound |
| BuiltinFunction.cpp | 362 | builtinFunctionConstructor |
| BuiltinAtomics.cpp | 362 | builtinAtomicsLoad |
| BuiltinWeakRef.cpp | 362 | builtinWeakRefConstructor |
| BuiltinIntl.cpp | 476 | builtinIntlCollatorConstructor |
| BuiltinGeneratorFunction.cpp | 362 | builtinGeneratorFunction |
| BuiltinAsyncFunction.cpp | 362 | builtinAsyncFunction |
| BuiltinAsyncFromSyncIterator.cpp | 476 | builtinAsyncFromSyncIteratorNext |

**v5·v6·v7과 중복 없음:** v5(Array, TypedArray, ArrayBuffer, String, Object, DataView, Number, Map), v6(Set, Date, Boolean, JSON, RegExp, Error, Symbol, Iterator), v7(WeakMap, WeakSet, Promise, Reflect, BigInt, Proxy, SharedArrayBuffer, FinalizationRegistry). v8은 Math, Function, Atomics, WeakRef, Intl, GeneratorFunction, AsyncFunction, AsyncFromSyncIterator.
