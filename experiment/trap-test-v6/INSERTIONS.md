# trap-test-v6 함정 코드 삽입 위치 (완전 신규)

v4/v5와 **다른 파일·다른 함수**에만 삽입. 주석 없음.

---

## 1. BuiltinSet.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 476 | `builtinSetForEach` | `thisArgObj = (argc >= 2 && argv[1].isObject()) ? argv[1].asObject() : nullptr` 후, 루프 전에 `(void)thisArgObj->getPrototype(state);` (null 미검사) |

---

## 2. BuiltinDate.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 369 | `builtinDateUTC` | `args[]`에서 year 등 읽은 직후 `scale = (argc > 0) ? argv[0].toInteger(state) : 1.0`, `(void)(year / scale);` (scale 0 가능) |

---

## 3. BuiltinBoolean.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinBooleanConstructor` | 전역 `static size_t s_booleanConstructCount = 0;`, 생성자 맨 앞에 `s_booleanConstructCount++;` (동기화 없음) |

---

## 4. BuiltinJSON.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 401 | `builtinJSONRawJSON` | `char* parseScratch = new char[48];`를 empty 검사 직전에 할당. 빈 문자열·wrong code point·parse 예외·isObject 시 throw하여 return 전에 `delete[]` 못 함. 정상 경로 끝에만 `delete[] parseScratch;` |

---

## 5. BuiltinRegExp.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinRegExpConstructor` | 전역 `static size_t s_regExpConstructCount = 0;`, 생성자 맨 앞에 `s_regExpConstructCount++;` (동기화 없음) |

---

## 6. BuiltinError.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinErrorConstructor` | 전역 `static size_t s_errorConstructCount = 0;`, 생성자 맨 앞에 `s_errorConstructCount++;` (동기화 없음) |

---

## 7. BuiltinSymbol.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 476 | `builtinSymbolConstructor` | `descStr` 설정 후(undefined면 nullptr), `(void)descStr->length();` 호출 (descStr이 null일 수 있음) |

---

## 8. BuiltinIterator.cpp

| CWE | 함수 | 삽입 내용 |
|-----|------|-----------|
| 362 | (전역) + `builtinIteratorFrom` | 전역 `static size_t s_iteratorFromCallCount = 0;`, 함수 맨 앞에 `s_iteratorFromCallCount++;` (동기화 없음) |

---

## 요약

| 파일 | CWE | 함수 |
|------|-----|------|
| BuiltinSet.cpp | 476 | builtinSetForEach |
| BuiltinDate.cpp | 369 | builtinDateUTC |
| BuiltinBoolean.cpp | 362 | builtinBooleanConstructor |
| BuiltinJSON.cpp | 401 | builtinJSONRawJSON |
| BuiltinRegExp.cpp | 362 | builtinRegExpConstructor |
| BuiltinError.cpp | 362 | builtinErrorConstructor |
| BuiltinSymbol.cpp | 476 | builtinSymbolConstructor |
| BuiltinIterator.cpp | 362 | builtinIteratorFrom |

**v5와 중복 없음:** v5는 Array, TypedArray, ArrayBuffer, String, Object, DataView, Number, Map. v6는 Set, Date, Boolean, JSON, RegExp, Error, Symbol, Iterator 전부 다른 builtin.
