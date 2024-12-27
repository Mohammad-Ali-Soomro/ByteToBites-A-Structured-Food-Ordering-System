# ByteToBites-A-Structured-Food-Ordering-System

# Food Ordering and Delivery System

A comprehensive C++ application for managing restaurant orders, deliveries, and user interactions.

## Features

### User Roles
- **Customer**: Place orders, track deliveries, manage account
- **Rider**: Handle deliveries, update order status
- **Admin**: Manage system, view statistics, handle menu

### Core Functionality

#### 1. Order Management
- Browse menu items
- Place and track orders
- Real-time order status updates
- Bill generation
- Balance checking before order placement

#### 2. Menu System
- View full menu
- Filter by food type
- Dynamic price calculation
- Add/remove menu items (Admin)

#### 3. Delivery System
- Rider assignment
- Real-time delivery tracking
- Route management
- Delivery status updates

#### 4. User Management
- Secure login system
- Balance management
- Password changes
- User profile management

#### 5. Admin Dashboard
- View system statistics
- Manage riders
- View customer reports
- Handle menu updates
- Monitor order flow

### File Structure
project/
├── auth/
│ ├── loginPage.h
│ ├── users.txt
│ ├── riders.txt
│ └── admins.txt
├── menu/
│ ├── menu.h
│ └── menu.txt
├── orderStatus/
│ ├── orders.h
│ ├── orders.txt
│ └── bills.txt
└── Home.cpp



### Data Structures Used
- **Linked Lists**: Order management
- **Vectors**: Menu items storage
- **Maps**: Statistical analysis
- **File Handling**: Data persistence

### Key Classes
- `CurrentUser`: User session management
- `Menu`: Menu operations
- `MenuItem`: Individual menu items
- `MyOrder`: Order processing
- `Delivery`: Delivery management

## Technical Requirements
- C++ compiler
- File system access
- Standard Template Library (STL)

## Setup Instructions
1. Clone repository
2. Configure file paths in source code
3. Compile using C++ compiler
4. Create necessary text files
5. Run the executable


