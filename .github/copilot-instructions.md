# Airline Reservation System (ARS) - Copilot Instructions

## Project Overview
This is a **console-based Airline Reservation and Management System** built in C++17 using CMake. The system implements role-based access control with three user types (Administrator, BookingAgent, Passenger) and follows a layered architecture pattern with domain-driven design principles.

## Architecture & Structure

### **Layer Pattern (4-tier architecture)**
```
src/
├── domain/       # Business entities (User, Flight, Reservation, Aircraft, etc.)
├── repository/   # Data access layer (JSON-based persistence in data/)
├── services/     # Business logic (auth, booking, checkin, flight, passenger, reporting)
└── ui/           # Console interface (role-specific menus: admin/, agent/, passenger/)
```

**Critical Flow**: `main.cpp` → AuthService → Role-based Menu (AdminMenu/AgentMenu/PassengerMenu) → Services → Repositories → Domain

### **Domain Model Key Patterns**
- **Inheritance hierarchy**: Base `User` class with `Administrator`, `BookingAgent`, `Passenger` subclasses
- **Enum-driven state**: All domain entities use strongly-typed enums (`UserRole`, `FlightStatus`, `ReservationStatus`)
- **Shared ownership**: Use `shared_ptr` for cross-referencing entities (e.g., `Reservation` holds `shared_ptr<Flight>` and `shared_ptr<Passenger>`)

## Build System

### **Primary Build Method: CMake**
```bash
# From AirlineSystem/ directory:
mkdir build && cd build
cmake ..
cmake --build .
# Executable outputs to: build/bin/AirlineSystem.exe
```

**Important**: CMakeLists.txt explicitly lists all source files - **you must update `set(SOURCES ...)` when adding new .cpp files**.

### **Makefile** (currently empty - CMake is the active build system)

## Data Persistence

### **JSON Storage Pattern**
- All data lives in `data/` directory: `users.json`, `flights.json`, `reservations.json`
- JSON structure mirrors domain class hierarchies with role-specific fields
- Example user JSON structure:
  ```json
  {
    "userId": "U1",
    "username": "adminUser", 
    "passwordHash": "hashed_admin123",
    "fullName": "System Admin",
    "role": "Administrator",  // Maps to UserRole enum
    "active": true,
    "adminLevel": 1           // Role-specific field
  }
  ```

### **Repository Pattern**
- Repositories load from JSON on construction: `UserRepository userRepo("data/users.json");`
- Use `findByUsername()` / `findByUserId()` for queries
- No ORM - direct JSON parsing expected

## Code Conventions

### **Naming Standards**
- **Classes**: PascalCase (`AdminMenu`, `FlightAttendant`)
- **Files**: Match class name exactly (e.g., `AdminMenu.h`/`AdminMenu.cpp`)
- **Enums**: Scoped `enum class` with PascalCase values (`UserRole::Administrator`)
- **Members**: camelCase (`userId`, `flightNumber`)

### **Header Guards**
Use traditional `#ifndef` pattern:
```cpp
#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H
// ... code ...
#endif
```

### **Include Style**
- Use relative paths from `src/`: `#include "../../domain/user/User.h"`
- Always add `using namespace std;` after includes (project convention)
- Include order: domain headers → repository → services → utilities

### **Memory Management**
- **Factory ownership**: Use `make_unique` for menu creation in main.cpp
- **Shared entities**: Domain models use `shared_ptr` for relationships
- **RAII**: All resources managed through smart pointers

## UI/UX Patterns

### **Menu System**
- Abstract base class `Menu` with `display()` and `handleInput(int choice)`
- Each role has dedicated menu class inheriting from `Menu`
- Menu loop in main.cpp: display → read choice → handle → repeat until logout
- Placeholder pattern for unimplemented features: `cout << "\n[TODO] Feature Name - Coming in Phase X\n";`

### **Input/Output Style** (per `Input and expected output.md`)
- Use ASCII art separators: `========================================`
- Section headers with dashes: `--- Administrator Menu ---`
- Prompt format: `Enter choice: ` (with trailing space)
- Date/time format: `YYYY-MM-DD HH:MM`
- Clear success messages after operations

## Key Implementation Notes

1. **Authentication**: Currently uses plain-text password comparison (marked as "Temporary" in AuthService) - hashing intended for future phase

2. **Phase-based development**: Comments indicate features "Coming in Phase 3/4" - check context when implementing placeholder TODO functions

3. **Role validation**: After login, main.cpp validates user's actual role matches selected role before showing menu

4. **Services directory**: Contains subdirectories (`auth/`, `booking/`, `checkin/`, etc.) but many are empty - follow AuthService pattern when implementing new services

5. **No external dependencies**: Project uses standard library only - no Boost, JSON libraries, or external parsers mentioned

## Common Tasks

### **Adding a new domain entity**
1. Create header/cpp in appropriate `domain/` subdirectory
2. Update CMakeLists.txt `SOURCES` section
3. Follow enum + shared_ptr pattern from existing entities

### **Implementing a TODO menu function**
1. Reference `Input and expected output.md` for expected UX flow
2. Create appropriate service class if needed
3. Follow display → input → validate → action → confirm pattern

### **Adding role-specific features**
- User subclasses (Administrator/BookingAgent/Passenger) should add role-specific methods
- Keep base `User` class minimal (authentication/identification only)
- Access control happens in menu layer, not domain layer

## Running the System
Execute from `build/bin/` after building. The system expects `data/` directory at the original project root, so run from AirlineSystem directory or ensure relative path to data files is correct.
