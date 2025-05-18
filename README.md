# 🍞 Advanced Bakery Simulation (Multiprocessing + OpenGL)

This project is a Linux-based simulation of a **Bakery Management System** using **multiprocessing** and **OpenGL** for visualization. The simulation models a complete bakery workflow with multiple process types that communicate and coordinate to fulfill customer orders.

The system consists of several types of processes:

- `main`: Central process that initializes and manages the entire simulation.
- `chef`: Prepares food items based on team specialization (paste, cake, sandwich, sweets, patisseries).
- `baker`: Bakes prepared items (breads, cakes, patisseries).
- `seller`: Handles customer orders.
- `supply_chain`: Ensures ingredients are available and replenished.
- `customer`: Simulates customer behavior and order generation.
- `graphics`: Handles visualization using OpenGL.

---

## 📦 Features

- Multi-process communication using **message queues**, **shared memory**, and **mutexs**.
- Dynamic configuration from text files (or use defaults).
- Real-time visualization with **OpenGL + GLFW + GLAD + FreeType**.
- Order fulfillment mechanics and randomized behaviors.
- Comprehensive inventory tracking and product lifecycle management.
- Customizable in-game font (replace **fonts/arial.ttf**).

---

## ⚙️ Configuration Files

The simulation can be configured through various configuration files. If not provided, default configurations will be generated.

### `config.txt`

```
# Default configuration for Bakery Simulation

# Number of each type of product
NUM_BREAD_TYPES=3
NUM_SANDWICH_TYPES=3
NUM_CAKE_FLAVORS=2
NUM_SWEET_FLAVORS=4
NUM_SWEET_PATISSERIE_TYPES=3
NUM_SAVORY_PATISSERIE_TYPES=2

# Number of employees
NUM_PASTE_CHEFS=2
NUM_CAKE_CHEFS=2
NUM_SANDWICH_CHEFS=2
NUM_SWEETS_CHEFS=2
NUM_SWEET_PATISSERIE_CHEFS=2
NUM_SAVORY_PATISSERIE_CHEFS=2
NUM_CAKE_BAKERS=2
NUM_PATISSERIE_BAKERS=2
NUM_BREAD_BAKERS=2
NUM_SELLERS=3
NUM_SUPPLY_CHAIN=2

# Ingredient purchase ranges
WHEAT_PURCHASE_MIN=50
WHEAT_PURCHASE_MAX=100
YEAST_PURCHASE_MIN=20
YEAST_PURCHASE_MAX=50
BUTTER_PURCHASE_MIN=30
BUTTER_PURCHASE_MAX=80
MILK_PURCHASE_MIN=40
MILK_PURCHASE_MAX=90
SUGAR_PURCHASE_MIN=30
SUGAR_PURCHASE_MAX=80
SALT_PURCHASE_MIN=10
SALT_PURCHASE_MAX=30
SWEET_ITEMS_PURCHASE_MIN=20
SWEET_ITEMS_PURCHASE_MAX=60
CHEESE_PURCHASE_MIN=30
CHEESE_PURCHASE_MAX=70
SALAMI_PURCHASE_MIN=30
SALAMI_PURCHASE_MAX=70

# Product prices
BREAD_PRICE_1=2.50
BREAD_PRICE_2=3.00
BREAD_PRICE_3=3.50
SANDWICH_PRICE_1=4.00
SANDWICH_PRICE_2=4.50
SANDWICH_PRICE_3=5.00
CAKE_PRICE_1=15.00
CAKE_PRICE_2=18.00
SWEET_PRICE_1=1.50
SWEET_PRICE_2=1.75
SWEET_PRICE_3=2.00
SWEET_PRICE_4=2.25
SWEET_PATISSERIE_PRICE_1=3.50
SWEET_PATISSERIE_PRICE_2=4.00
SWEET_PATISSERIE_PRICE_3=4.25
SAVORY_PATISSERIE_PRICE_1=3.00
SAVORY_PATISSERIE_PRICE_2=3.75

# Simulation parameters
FRUSTRATED_CUSTOMER_THRESHOLD=10
COMPLAINING_CUSTOMER_THRESHOLD=5
MISSING_ITEM_REQUEST_THRESHOLD=15
PROFIT_THRESHOLD=1000.00
SIMULATION_TIME_MINUTES=1 # Short time for testing

# Customer simulation parameters
CUSTOMER_ARRIVAL_RATE_MIN=5  # Customers per minute
CUSTOMER_ARRIVAL_RATE_MAX=15
CUSTOMER_PATIENCE_MIN=10 # Seconds
CUSTOMER_PATIENCE_MAX=60
COMPLAINT_PROBABILITY=0.10
```

---

## 🛠️ Build Instructions (Linux)

Make sure you have the required libraries installed:

### ✅ Dependencies

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev libfreetype6-dev
```

### 🔨 Build the Programs

#### To compile all components in debug mode

```bash
make
```

Compiled binaries will be placed in the Project directory.

To run the simulation:

```bash
./bakery_sim
```

---

## 🧁 System Architecture

The system uses several IPC (Inter-Process Communication) mechanisms:

1. **Shared Memory Segments**:
   - `Inventory`: Raw ingredients tracking
   - `UnbakedProducts`: Items prepared but not yet baked
   - `Products`: Completed products ready for sale
   - `Statistics`: Runtime statistics and metrics
   - `ProcessStatus`: Real-time status of all processes

2. **Message Queues**:
   - `orders_msqid`: Customer orders and confirmations
   - `ingredients_msqid`: Requests for ingredient replenishment
   - `notifications_msqid`: Inter-process notifications

3. **mutexs**: Protecting access to shared resources

---

## 🍰 Process Workflow

1. **Customer** generates orders which are sent to a **Seller**.
2. **Seller** checks if products are available:
   - If available: Completes the sale and updates inventory.
   - If unavailable: Notifies appropriate **Chef** or **Baker**.
3. **Chef** prepares food items if ingredients are available:
   - If ingredients are low: Requests more from **Supply Chain**.
   - When preparation is complete: Updates `UnbakedProducts` if baking is needed or `Products` if ready for sale.
4. **Baker** bakes prepared items from `UnbakedProducts` and moves them to `Products`.
5. **Supply Chain** replenishes raw ingredients in the `Inventory`.

The **Graphics** process provides real-time visualization of the entire bakery operation.

---

## 🖥️ Visualization

The graphical interface shows:
- Current inventory levels
- Process status for all workers
- Customer orders and fulfillment status
- Sales statistics and performance metrics

![Bakery Simulation](screenshot.png "Bakery Simulation Screen")

---

## 🚀 Advanced Usage

### Performance Tuning

Adjust the number of processes in each category to optimize for different scenarios:
- High customer arrival frequency: Increase the number of sellers
- Complex orders: Increase the number of specialized chefs and bakers
- Supply chain issues: Increase the number of supply chain processes

### Custom Product Configurations

Modify the product counts, varieties, and prices in the configuration file to simulate different bakery business models.

---

## 📊 Monitoring & Statistics

The system tracks various performance metrics:
- Total sales and profit
- Order fulfillment rate
- Process utilization
- Ingredient consumption patterns
- Missing item requests

These statistics can be used to analyze the efficiency of the bakery operation and identify bottlenecks.