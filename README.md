# C++ Banking and Authentication System

A C++17 command-line banking simulation for practicing object-oriented programming, account management, authentication, input validation, and file persistence. The application uses OpenSSL for password hashing and includes automated tests for its core behavior.

**Educational project only.** This is not a real banking system. Do not enter real financial information, identity numbers, or passwords you use elsewhere.

## Features

- Create an account and log in with a username and password.
- View a balance, deposit funds, and withdraw funds.
- Change a password after verifying the current password.
- Save accounts in a local text file and load them when the program starts again.
- Store salted password hashes rather than plaintext passwords.
- Represent money as integer cents to avoid floating-point rounding in balance calculations.
- Validate user input and reject malformed monetary values or database records.
- Run automated tests for input handling, hashing, transactions, and saving/loading accounts.

## Project structure

```text
Cpp-Banking-and-Authentication/
├── CMakeLists.txt
├── README.md
├── LEARNING_STEPS.md
├── src/
│   ├── main.cpp                 Menu and program flow
│   ├── Account.h / Account.cpp  Account state and operations
│   ├── AccountManager.h / AccountManager.cpp  Account collection and file storage
│   ├── Crypto.h / Crypto.cpp    OpenSSL password hashing and byte conversion
│   └── InputHandling.h / InputHandling.cpp  Input validation and money formatting
└── tests/
    └── test_core.cpp            Automated core tests
```

The program is divided into small modules so each part has a specific responsibility:

1. **`main.cpp`** displays menus and coordinates user actions.
2. **`InputHandling`** checks input and converts amounts such as `"125.5"` into `12550` cents.
3. **`Account`** stores account information and implements deposit, withdrawal, and password verification.
4. **`AccountManager`** manages a `std::vector<Account>` and loads or saves account data.
5. **`Crypto`** uses OpenSSL to generate random salts and derive PBKDF2-HMAC-SHA256 password hashes.

## Build and run

**Requirements:** CMake 3.16 or newer, a C++17 compiler, and OpenSSL development libraries.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/secure_bank
```

The executable uses a `data/` folder relative to the working directory. Passwords are visible while typed in the console; this project does not implement masked terminal input.

## Storage and password security

The local database starts with a `BANK_DEMO_V1` header. Each account is saved using this structure:

```text
username|salt_hex|hash_hex|balance_cents
```

The password itself is not stored. OpenSSL's PBKDF2-HMAC-SHA256 function derives a hash using a randomly generated salt. This project uses a fixed work factor of 600,000 iterations. Changing that constant would require a migration strategy for previously saved accounts, because the current database format does not store an iteration count per account.

## Testing

`tests/test_core.cpp` checks username and password validation, money parsing, password hashing, deposits and withdrawals, password changes, saving/loading accounts, and malformed database records. The GitHub Actions workflow also configures and builds the project, runs CTest, and checks that the program starts and exits.

## Scope and limitations

This is a single-user, local educational simulation. It deliberately focuses on core C++ and security concepts rather than production banking infrastructure.

- The local database is neither encrypted nor protected against deliberate file tampering.
- There is no login rate limiting, multi-factor authentication, or password masking.
- It does not provide concurrent account access, a transaction ledger, or production-grade recovery.
- Saving through a temporary file and rename reduces some write-failure risks, but it is not a transactional database; cross-platform replacement details may differ.
- Input rules and authorization are intentionally limited to this console demonstration.

## Learning goals

This project provides hands-on practice with classes and constructors, vectors, references and pointers, functions, file streams, error handling, OpenSSL integration, and automated C++ testing. `LEARNING_STEPS.md` provides exercises for understanding and extending each module.
