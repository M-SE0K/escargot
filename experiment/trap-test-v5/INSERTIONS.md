# trap-test-v5 함정 코드 삽입 위치

주석 없이 삽입된 CWE 함정 목록. (코드 내 CWE/버전 주석 없음)

---

## 1. BuiltinArrayBuffer.cpp

| # | CWE | 함수 | 삽입 위치 (대략 라인) | 삽입 내용 요약 |
|---|-----|------|----------------------|----------------|
| 1 | 401 | `builtinArrayBufferTransfer` | 할당 직후 ~ early return | `char* transferScratch = new char[32];` 후 `newByteLength == 0`이면 빈 버퍼 생성 후 `return emptyBuf` (scratch 미해제). 정상 경로 끝에 `delete[] transferScratch;` |
| 2 | 190 | `builtinArrayBufferSlice` | newLen 계산 | `size_t newLen = static_cast<size_t>(final_ - first);` 후 `if ((int)final_ - (int)first < 0) newLen = 0;` (무검사 캐스트 후 보정) |

---

## 2. BuiltinString.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 3 | 125 | `builtinStringIndexOf` | `start` 계산 직후, `str->find` 호출 전 | `searchStr->length() == 0 && start <= len` 일 때 `(void)str->charAt(start);` (start==len이면 OOB) |
| 4 | 369 | `builtinStringRepeat` | `count` 계산 직후 | `double divisor = (argc > 1) ? argv[1].toInteger(state) : 1.0;` 및 `(void)(str->length() / divisor);` (divisor 0 가능) |

---

## 3. BuiltinObject.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 5 | 362 | (전역) + `builtinObjectKeys` | 파일 상단 typedef 다음, `builtinObjectKeys` 내부 | 전역 `static size_t s_objectKeysCallCount = 0;` 선언, 함수 진입 시 `s_objectKeysCallCount++;` (동기화 없음) |

---

## 4. BuiltinDataView.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 6 | 190 | DataView 생성자 (byteLength 있을 때) | viewByteLength 검사 블록 | `size_t totalBytes = static_cast<size_t>(offset) + static_cast<size_t>(viewByteLength.value());` 후 `if (totalBytes > bufferByteLegnth)` (오버플로우 무검사 덧셈) |

---

## 5. BuiltinNumber.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 7 | 369 | `builtinNumberToFixed` | digit 범위 검사 직후 | `double divisor = (argc > 1) ? argv[1].toInteger(state) : 1.0;` 및 `(void)(number / divisor);` (divisor 0 가능) |

---

## 6. BuiltinMap.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 8 | 476 | `builtinMapForEach` | T 설정 직후, entries 사용 전 | `Object* receiverObj = (argc >= 2 && argv[1].isObject()) ? argv[1].asObject() : nullptr;` 후 `(void)receiverObj->getPrototype(state);` (null 미검사) |

---

## 7. BuiltinArray.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 9 | 362 | (전역) + `builtinArrayEvery` | CHECK_ARRAY_LENGTH 매크로 다음, every 내부 | 전역 `static size_t s_diagnosticCount = 0;`, every에서 `s_diagnosticCount++;` (동기화 없음) |
| 10 | 369 | `builtinArraySplice` | actualStart 계산 직후 | `double divisor = (argc > 1) ? argv[1].toInteger(state) : 1.0;` 및 `(void)(len / divisor);` (divisor 0 가능) |
| 11 | 476 | `builtinArrayEvery` | T 설정 부분 | `Object* receiverObj = (argc > 1 && argv[1].isObject()) ? argv[1].asObject() : nullptr;`, `Value T = Value(receiverObj);` (null일 수 있는 포인터로 Value 생성) |

---

## 8. BuiltinTypedArray.cpp

| # | CWE | 함수 | 삽입 위치 | 삽입 내용 요약 |
|---|-----|------|-----------|----------------|
| 12 | 401 | `builtinTypedArrayFrom` (iterator 경로) | len 계산 직후 | `char* scratch = new char[64];`, `len == 0`이면 `return targetObj` (scratch 미해제). 루프 끝나고 `delete[] scratch;` |
| 13 | 20 | `builtinTypedArrayFrom` (arrayLike 경로) | len 다음 | `size_t rawLen = (size_t)arrayLike->get(...).value(...).toNumber(state);` 후 루프 상한을 `rawLen`으로 사용 (ToLength 미적용) |
| 14 | 787 | copyWithin 루프 | setValueInBuffer 호출 직전 | `size_t writeIndex = (direction == 1 && countBytes == 1) ? (toByteIndex + 1) : toByteIndex;` 후 `setValueInBuffer(..., writeIndex, ...)` (마지막 바이트 시 OOB write) |
| 15 | 190 | copyWithin 바이트 인덱스 계산 | (기존 코드 유지) | `toByteIndex = targetIndex * elementSize + byteOffset` 등 무검사 곱셈 (이미 존재, 추가 수정 없음) |
| 16 | 125 | `fastTypedArrayIndexSearch` | 함수 맨 끝 return 직전 | `(void)buffer[byteLength];` 후 `return Value(-1);` (값 못 찾을 때 byteLength 위치 읽기, OOB) |

---

## 요약

| 파일 | 함정 개수 | CWE |
|------|-----------|-----|
| BuiltinArrayBuffer.cpp | 2 | 401, 190 |
| BuiltinString.cpp | 2 | 125, 369 |
| BuiltinObject.cpp | 1 | 362 |
| BuiltinDataView.cpp | 1 | 190 |
| BuiltinNumber.cpp | 1 | 369 |
| BuiltinMap.cpp | 1 | 476 |
| BuiltinArray.cpp | 3 | 362, 369, 476 |
| BuiltinTypedArray.cpp | 5 | 401, 20, 787, 190, 125 |
| **합계** | **16** | 8종 CWE |

위 위치에 주석 없이 삽입되어 있음.
