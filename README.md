# Secure C++ Banking Demo — Intermediate Learning Version

A deliberately smaller **educational** C++17 console banking project, designed as a realistic learning bridge between an unfinished March 30 prototype and a much more elaborate later project.

This is **not a real banking system**. Do not enter real banking details, identity numbers, or passwords that you use elsewhere.

## What it does

- Create accounts and log in with a username and password.
- View a balance, deposit money, and withdraw money.
- Change the password after checking the current one.
- Save accounts in a simple local text file and load them on the next run.
- Store only a salted password hash, not the plaintext password.
- Use integer cents instead of floating-point dollars.
- Reject malformed money input and invalid saved records.
- Run automated C++ tests for input handling, hashing, balances, saving, and loading.

## What was deliberately removed

Compared with the more advanced project, this version has **no separate BankingUI class/module**, no AuditLogger, no login-attempt map or lockout timer, no multi-version password work factor, and no ID-number fields. It uses **one account vector and one simple file format**. These are deliberate scope reductions, not claims that the removed protections are unnecessary in production.

## Project layout

```text
CMakeLists.txt
src/
  main.cpp               menus and program flow
  Account.h / .cpp       one account's data and operations
  AccountManager.h / .cpp  vector of accounts and save/load
  Crypto.h / .cpp        random salt and PBKDF2 password hashing
  InputHandling.h / .cpp  validation, money parsing, input
 tests/
  test_core.cpp          automated tests
.github/workflows/ci.yml automatic build and tests
```

## Build and run

Requirements: CMake 3.16+, a C++17 compiler, and OpenSSL development libraries.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/secure_bank
```

The program creates a `data/` folder in the working directory. Password input is **visible as typed** in the console; this is a learning demo, not a secure password terminal.

## How the files connect

1. `main.cpp` shows menus and calls the other modules.
2. `InputHandling` reads input and converts strings such as `"125.5"` to `12550` cents.
3. `AccountManager` looks for accounts inside a `std::vector<Account>` and saves changes.
4. `Account` owns the balance, salt, and password hash.
5. `Crypto` asks OpenSSL to generate random bytes and to derive a PBKDF2-HMAC-SHA256 password hash.

The database format is `BANK_DEMO_V1` followed by lines of `username|salt_hex|hash_hex|balance_cents`. The PBKDF2 iteration count (600,000) is a fixed constant in this learning version. If you change it later, **old saved account passwords will stop verifying** unless you also design a credential migration or record per-account work factors.

## Limitations to explain honestly

- Local unencrypted, unauthenticated database: someone with filesystem access can tamper with data.
- No login throttling, two-factor authentication, or terminal masking.
- No concurrent-user support or reliable transaction ledger.
- Temporary-file replacement reduces some accidental data loss but is not a full transactional database; this approach is tested on Linux and is intended for Linux/macOS filesystems. Windows file replacement may require additional handling.
- Username/password validation is deliberately simple.
- Never use this with real personal or financial information.

## How to use it as *your* resume project

Study and recreate the five modules yourself. Use this implementation as a reference, not as evidence that you independently authored code you have not yet written. A fair resume description after you have reproduced and tested the key logic is:

> Built a modular C++17 banking simulation featuring OpenSSL PBKDF2 password hashing, salted credential storage, validated input, integer-cent transactions, and file-based persistence; tested with CMake/CTest.

## Suggested learning order

1. `InputHandling.cpp`: functions, string methods, loops, and validation.
2. `Account.cpp`: constructors, vectors, class members, and transactions.
3. `Crypto.cpp`: basic OpenSSL calls and byte vectors.
4. `AccountManager.cpp`: looping through vectors, finding objects, file input/output.
5. `main.cpp`: how the program connects the functions.
6. `tests/test_core.cpp`: add another test yourself.
