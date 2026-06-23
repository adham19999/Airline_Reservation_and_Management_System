# Airline Reservation and Management System

A C++ console application for airline operations management, covering flight scheduling, aircraft and crew assignment, bookings, payments, and a passenger loyalty program. The system is built around a layered, repository-based architecture with JSON file persistence.

<p align="left">
  <img alt="language" src="https://img.shields.io/badge/Language-C%2B%2B17-orange?style=flat-square">
  <img alt="framework" src="https://img.shields.io/badge/Framework-Console%20Application-green?style=flat-square">
  <img alt="concepts" src="https://img.shields.io/badge/Concepts-OOP%20%7C%20Repository%20Pattern%20%7C%20JSON%20Persistence-9cf?style=flat-square">
  <img alt="status" src="https://img.shields.io/badge/Status-Complete-success?style=flat-square">
</p>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [How to Run](#how-to-run)
- [Key Concepts](#key-concepts)
- [Author](#author)

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Overview

The system models the core operations of an airline: managing flights and the aircraft assigned to them, assigning and tracking crew, handling passenger bookings and payments, and maintaining loyalty accounts. Three user roles drive the application's permissions and menus: Administrator, Booking Agent, and Passenger.

Each domain entity (flights, aircraft, reservations, payments, crew, users) is backed by its own repository, which loads from and persists to a dedicated JSON file. Business rules — fare calculation, seat availability, crew flight-hour limits, loyalty point accrual — are encapsulated in dedicated service classes, separating logic from data access and from the console interaction layer.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Features

- Role-based menus for Administrator, Booking Agent, and Passenger
- Flight management: creation, scheduling, updates, and status changes
- Aircraft management with seat layout and maintenance record tracking
- Crew management with role-based assignment and flight-hour limit enforcement
- Booking workflow: search flights, reserve seats, process payments, check in
- Passenger loyalty program with point accrual tied to completed flights
- Reporting service for operational summaries
- JSON-backed persistence with no external database dependency
  
<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Architecture

The codebase is organized into four logical layers:

- **Core entities** (`include/core`, `src/core`) — `Flight`, `Aircraft`, `Seat`, `SeatLayout`, `LoyaltyAccount`, `MaintenanceRecord`
- **Users and crew** (`include/users`, `include/crew`) — `Admin`, `BookingAgent`, `Passenger`, `Pilot`, `FlightAttendant`, `CrewAssignment`
- **Booking domain** (`include/booking`) — `Reservation`, `Payment`, `BoardingPass`
- **Repositories and services** (`include/repositories`, `include/services`) — JSON-backed data access and business logic: `FlightService`, `BookingService`, `PaymentService`, `CrewMember` assignment rules, `LoyaltyService`, `MaintenanceService`, `CheckInService`, `ReportService`, `AuthenticationService`

```mermaid
graph LR
    A["Console Menu - Role Based"] --> B["Services"]
    B --> C["Repositories"]
    C --> D["JSON Data Files"]
    B --> E["Core Entities"]
    E --> C
```

## Project Structure

```
AirlineSystem/
├── include/
│   ├── core/             # Flight, Aircraft, Seat, LoyaltyAccount, MaintenanceRecord
│   ├── users/             # Admin, BookingAgent, Passenger, User
│   ├── crew/              # Pilot, FlightAttendant, CrewAssignment
│   ├── booking/           # Reservation, Payment, BoardingPass
│   ├── repositories/       # JSON-backed repositories per entity
│   └── services/          # Business logic services
├── src/                   # Implementation files mirroring include/
├── data/                  # JSON data files (flights, aircraft, crew, users,
│                           # reservations, payments)
├── third_party/           # nlohmann::json, picosha2
└── main.cpp                # Console entry point and role-based menu loop
```

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## How to Run

**Requirements**

- A C++17 compiler (GCC, Clang, or MSVC)

**Build**

```bash
g++ -std=c++17 -Iinclude -Ithird_party src/**/*.cpp main.cpp -o AirlineSystem
```

**Run**

```bash
./AirlineSystem
```

The application starts at a login/role-selection prompt. Use the in-menu registration option to create an Administrator, Booking Agent, or Passenger account.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Key Concepts

- Object-oriented design: encapsulation, inheritance, and polymorphism across user roles and crew types
- Repository pattern for JSON-backed persistence, decoupled from business logic
- Service layer encapsulating business rules (fare calculation, seat availability, crew flight-hour limits, loyalty accrual)
- File-based data persistence with no external database dependency

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=gradient&height=2&section=header&customColorList=12"/>

## Author

Adham — GitHub: [adham19999](https://github.com/adham19999)
