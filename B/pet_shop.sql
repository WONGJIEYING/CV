-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jan 14, 2024 at 12:49 PM
-- Server version: 8.0.30
-- PHP Version: 8.1.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `pet_shop`
--

-- --------------------------------------------------------

--
-- Table structure for table `adoption_record`
--

CREATE TABLE `adoption_record` (
  `Adoption_Pet_ID` varchar(10) NOT NULL,
  `Adoption_Pet_Name` varchar(50) DEFAULT NULL,
  `Adopter_Name` varchar(50) DEFAULT NULL,
  `Adoption_Date` date DEFAULT NULL,
  `Adopter_Contact` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `adoption_record`
--

INSERT INTO `adoption_record` (`Adoption_Pet_ID`, `Adoption_Pet_Name`, `Adopter_Name`, `Adoption_Date`, `Adopter_Contact`) VALUES
('AID00001', 'Daisy', 'Jimmy Wong', '2023-01-08', '012 339 8543'),
('AID00002', 'Bear', 'Shirley Yuen', '2023-01-08', '013 556 8064'),
('AID00003', 'Buddy', 'Justin Looi', '2023-01-08', '011 566 5470'),
('AID00004', 'Duke', 'Max Lee', '2023-01-08', '018 505 0186'),
('AID00005', 'Charlie', 'Carmen Kuo', '2023-01-08', '017 655 5621');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `Customer_ID` varchar(10) NOT NULL,
  `Customer_Name` varchar(50) DEFAULT NULL,
  `Phone` varchar(20) DEFAULT NULL,
  `Email` varchar(200) DEFAULT NULL,
  `Address` varchar(200) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`Customer_ID`, `Customer_Name`, `Phone`, `Email`, `Address`) VALUES
('CU00001', 'Terry', '010 789 2531', 'terry@gmail.com', 'Jalan Durian 23, Taman Indah'),
('CU00002', 'BuTong', '011 321 6513', 'butong@gmail.com', 'Jalan Sansan 77, Taman Mana'),
('CU00003', 'Jaspi', '012 325 4064', 'Jaspi@gmail.com', 'Jalan SamLong 88, Taman Mua'),
('CU00004', 'LianNi', '013 821 0653', 'LianNi@gmail.com', 'Jalan Yikyik 21, Taman Long'),
('CU00005', 'Nick', '014 021 5248', 'Nick@gmail.com', 'Jalan Jing 45, Taman Yu');

-- --------------------------------------------------------

--
-- Table structure for table `customer_do`
--

CREATE TABLE `customer_do` (
  `DO_No` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Delivery_Date` date DEFAULT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer_do`
--

INSERT INTO `customer_do` (`DO_No`, `Date`, `Delivery_Date`, `Customer_ID`, `Product_ID`, `Total_Quantity`) VALUES
('DO00001', '2023-08-01', '2023-08-02', 'CU00001', 'PD00001', 1),
('DO00002', '2023-08-01', '2023-08-02', 'CU00002', 'PD00002', 5),
('DO00003', '2023-08-01', '2023-08-02', 'CU00003', 'PD00003', 3),
('DO00004', '2023-08-01', '2023-08-02', 'CU00004', 'PD00004', 4),
('DO00005', '2023-08-01', '2023-08-02', 'CU00005', 'PD00005', 2);

-- --------------------------------------------------------

--
-- Table structure for table `customer_feedback`
--

CREATE TABLE `customer_feedback` (
  `Feedback_ID` varchar(10) NOT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Feedback_Text` varchar(500) DEFAULT NULL,
  `Feedback_Rating` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer_feedback`
--

INSERT INTO `customer_feedback` (`Feedback_ID`, `Customer_ID`, `Date`, `Feedback_Text`, `Feedback_Rating`) VALUES
('FB00001', 'CU00001', '2023-08-01', 'wish there were more choices', 6),
('FB00002', 'CU00002', '2023-08-02', 'The clerk is very friendly', 7),
('FB00003', 'CU00003', '2023-08-03', 'Pets are cute', 7),
('FB00004', 'CU00004', '2023-08-04', 'Pets are cute', 8),
('FB00005', 'CU00005', '2023-08-05', 'The service was excellent', 10);

-- --------------------------------------------------------

--
-- Table structure for table `customer_interest`
--

CREATE TABLE `customer_interest` (
  `Interest_ID` varchar(10) NOT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Interest_Level` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer_interest`
--

INSERT INTO `customer_interest` (`Interest_ID`, `Customer_ID`, `Product_ID`, `Interest_Level`) VALUES
('CI00001', 'CU00001', 'PD00001', 'High'),
('CI00002', 'CU00002', 'PD00002', 'High'),
('CI00003', 'CU00003', 'PD00003', 'Middle'),
('CI00004', 'CU00004', 'PD00004', 'Moderate'),
('CI00005', 'CU00005', 'PD00005', 'Moderate');

-- --------------------------------------------------------

--
-- Table structure for table `customer_invoice`
--

CREATE TABLE `customer_invoice` (
  `Invoice_No` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL,
  `Total_Selling_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer_invoice`
--

INSERT INTO `customer_invoice` (`Invoice_No`, `Date`, `Customer_ID`, `Product_ID`, `Total_Quantity`, `Total_Selling_Price`) VALUES
('IN00001', '2023-08-01', 'CU00001', 'PD00001', 1, '45.00'),
('IN00002', '2023-08-01', 'CU00002', 'PD00002', 1, '35.00'),
('IN00003', '2023-08-01', 'CU00003', 'PD00003', 3, '60.00'),
('IN00004', '2023-08-01', 'CU00004', 'PD00004', 3, '90.00'),
('IN00005', '2023-08-01', 'CU00005', 'PD00005', 1, '500.00');

-- --------------------------------------------------------

--
-- Table structure for table `customer_pet`
--

CREATE TABLE `customer_pet` (
  `Pet_ID` varchar(10) NOT NULL,
  `Pet_Name` varchar(50) DEFAULT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Description` text,
  `Breed` varchar(50) DEFAULT NULL,
  `Origin_Country` varchar(50) DEFAULT NULL,
  `Sex` varchar(10) DEFAULT NULL,
  `Color` varchar(20) DEFAULT NULL,
  `Register_Date` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `customer_pet`
--

INSERT INTO `customer_pet` (`Pet_ID`, `Pet_Name`, `Customer_ID`, `Description`, `Breed`, `Origin_Country`, `Sex`, `Color`, `Register_Date`) VALUES
('PID00001', 'Lola', 'CU00001', 'Small, white-coated, affectionate dogs', 'Maltese', 'South Korea', 'male', 'white', '2023-01-08'),
('PID00002', 'Max', 'CU00002', 'Lively, fluffy small dogs', 'Pomeranian', 'Taiwan', 'male', 'brown', '2023-01-08'),
('PID00003', 'Fluffy', 'CU00003', 'Elegant, social cats with blue eyes', 'Siamese', 'Thailand', 'female', 'blue point', '2023-01-08'),
('PID00004', 'Jeffrey', 'CU00004', 'Large, laid-back cats with blue eyes', 'Ragdoll', 'California', 'male', 'seal', '2023-01-08'),
('PID00005', 'Isabella', 'CU00005', 'Friendly, keen-scented hounds', 'Beagle', 'United States', 'female', 'black', '2023-01-08');

-- --------------------------------------------------------

--
-- Table structure for table `employees`
--

CREATE TABLE `employees` (
  `Employee_ID` varchar(10) NOT NULL,
  `Employee_Name` varchar(50) DEFAULT NULL,
  `Position` varchar(50) DEFAULT NULL,
  `Salary` decimal(10,2) DEFAULT NULL,
  `Certificate_Name` varchar(200) DEFAULT NULL,
  `Hiredate` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `employees`
--

INSERT INTO `employees` (`Employee_ID`, `Employee_Name`, `Position`, `Salary`, `Certificate_Name`, `Hiredate`) VALUES
('EM00001', 'Jayden', 'Admin Doctor', '3000.00', 'Certification in Pet Product Knowledge', '2023-10-01'),
('EM00002', 'Selina', 'Admin Doctor', '3000.00', 'Certification in Pet Nutrition', '2023-10-01'),
('EM00003', 'Gabriel', 'Driver', '2000.00', 'E Lessen', '2023-10-01'),
('EM00004', 'Angel', 'Customer Service', '2500.00', 'Certification in Pet Product Knowledge', '2023-10-01'),
('EM00005', 'Beatrice', 'Cashier', '2200.00', 'Certification in Pet Nutrition', '2023-10-01');

-- --------------------------------------------------------

--
-- Table structure for table `employee_training_certification`
--

CREATE TABLE `employee_training_certification` (
  `Training_ID` varchar(10) NOT NULL,
  `Employee_ID` varchar(10) DEFAULT NULL,
  `Certification_Date` date DEFAULT NULL,
  `Certificate_Name` varchar(50) DEFAULT NULL,
  `Training_Location` varchar(50) DEFAULT NULL,
  `Training_Address` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `employee_training_certification`
--

INSERT INTO `employee_training_certification` (`Training_ID`, `Employee_ID`, `Certification_Date`, `Certificate_Name`, `Training_Location`, `Training_Address`) VALUES
('TR00001', 'EM00001', '2023-10-01', 'Certification in Pet Product Knowledge', 'Animal Care School 1', 'Animal Care School 1, 333 Cedar Blvd, Countryside'),
('TR00002', 'EM00002', '2023-10-01', 'Certification in Pet Nutrition', 'Animal Care School 2', 'Animal Care School 2, 444 Birch Street, Riverside'),
('TR00003', 'EM00003', '2023-10-01', 'E Lessen', 'E Lesen Training Centre', 'E Lesen Training Centre, 789 Pine Street, Villagetown'),
('TR00004', 'EM00004', '2023-10-01', 'Certification in Pet Product Knowledge', 'Animal Care School 1', 'Animal Care School 1, 333 Cedar Blvd, Countryside'),
('TR00005', 'EM00005', '2023-10-01', 'Certification in Pet Nutrition', 'Animal Care School 2', 'Animal Care School 2, 444 Birch Street, Riverside');

-- --------------------------------------------------------

--
-- Table structure for table `inventory`
--

CREATE TABLE `inventory` (
  `Product_ID` varchar(10) NOT NULL,
  `Service_ID` varchar(10) DEFAULT NULL,
  `Accessories_ID` varchar(10) DEFAULT NULL,
  `Pet_Category_ID` varchar(10) DEFAULT NULL,
  `Vaccine_ID` varchar(10) DEFAULT NULL,
  `Stock_Quantity` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `inventory`
--

INSERT INTO `inventory` (`Product_ID`, `Service_ID`, `Accessories_ID`, `Pet_Category_ID`, `Vaccine_ID`, `Stock_Quantity`) VALUES
('PD00001', NULL, NULL, NULL, 'VA00001', 30),
('PD00002', NULL, 'AC00002', NULL, NULL, 30),
('PD00003', NULL, NULL, 'DOG00003', NULL, 30),
('PD00004', 'SE00004', NULL, NULL, NULL, 10000),
('PD00005', NULL, NULL, NULL, 'VA00005', 30);

-- --------------------------------------------------------

--
-- Table structure for table `lost_and_found_record`
--

CREATE TABLE `lost_and_found_record` (
  `Lost_Pet_ID` varchar(10) NOT NULL,
  `Lost_Pet_Name` varchar(50) DEFAULT NULL,
  `Description` text,
  `Location` varchar(50) DEFAULT NULL,
  `Finder_Name` varchar(50) DEFAULT NULL,
  `Finder_Contact` varchar(20) DEFAULT NULL,
  `Report_Date` date DEFAULT NULL,
  `Found_Date` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `lost_and_found_record`
--

INSERT INTO `lost_and_found_record` (`Lost_Pet_ID`, `Lost_Pet_Name`, `Description`, `Location`, `Finder_Name`, `Finder_Contact`, `Report_Date`, `Found_Date`) VALUES
('LID00001', 'Lucy', 'A white Persian cat with blue eyes', 'Setia Eco Park', 'Allison Ho', '012 648 8148', '2023-01-10', '2023-03-10'),
('LID00002', 'Anna', 'Found a brown Labrador Retriever', 'Cyberjaya Lake Gardens', 'Kendra Sow', '013 432 6563', '2023-01-10', '2023-03-10'),
('LID00003', 'Luna', 'Spotted a Siamese cat with blue eyes', 'Origami Park', 'Summer Yuen', '014 546 8970', '2023-01-10', '2023-03-10'),
('LID00004', 'Susan', 'Found a lost Ragdoll cat', 'Villea Port Dickson', 'Benjamin Chin', '015 627 0264', '2023-01-10', '2023-03-10'),
('LID00005', 'Jack', 'Discovered a stray Beagle', 'Bark Park', 'Christine Tay', '016 188 3254', '2023-01-10', '2023-03-10');

-- --------------------------------------------------------

--
-- Table structure for table `membership`
--

CREATE TABLE `membership` (
  `Membership_ID` varchar(10) NOT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Membership_Level` varchar(10) DEFAULT NULL,
  `Register_Date` date DEFAULT NULL,
  `Expire_Date` date DEFAULT NULL,
  `Debit` decimal(10,2) DEFAULT NULL,
  `Credit` decimal(10,2) DEFAULT NULL,
  `Last_Balance` decimal(10,2) DEFAULT NULL,
  `New_Balance` decimal(10,2) DEFAULT NULL,
  `Last_Point` decimal(10,2) DEFAULT NULL,
  `New_Point` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `membership`
--

INSERT INTO `membership` (`Membership_ID`, `Customer_ID`, `Membership_Level`, `Register_Date`, `Expire_Date`, `Debit`, `Credit`, `Last_Balance`, `New_Balance`, `Last_Point`, `New_Point`) VALUES
('ME00001', 'CU00001', 'VIP', '2023-01-10', '2024-01-10', '300.00', '30.00', '1000.00', '1270.00', '500.00', '530.00'),
('ME00002', 'CU00002', 'VVIP', '2023-01-10', '2024-01-10', '500.00', '50.00', '500.00', '950.00', '300.00', '700.00'),
('ME00003', 'CU00003', 'VVVIP', '2023-01-10', '2024-01-10', '1000.00', '60.00', '800.00', '1740.00', '400.00', '1380.00'),
('ME00004', 'CU00004', 'VIP', '2023-01-10', '2024-01-10', '300.00', '100.00', '900.00', '1100.00', '50.00', '150.00'),
('ME00005', 'CU00005', 'VVIP', '2023-01-10', '2024-01-10', '500.00', '70.00', '600.00', '1030.00', '600.00', '1340.00');

-- --------------------------------------------------------

--
-- Table structure for table `monthly_budget`
--

CREATE TABLE `monthly_budget` (
  `Monthly_Budget_ID` varchar(10) NOT NULL,
  `Purpose` varchar(50) DEFAULT NULL,
  `Description` text,
  `Budget` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `monthly_budget`
--

INSERT INTO `monthly_budget` (`Monthly_Budget_ID`, `Purpose`, `Description`, `Budget`) VALUES
('MB00001', 'Employee Salaries', 'Used to track payroll expenses paid to pet store employees', '100000.00'),
('MB00002', 'Inventory Purchases', 'Used to record spending on pet food, supplies and products', '15000.00'),
('MB00003', 'Advertising and Promotion', 'Spending on pet store promotions and promotions', '20000.00'),
('MB00004', 'Rent and Lease', 'Used to pay rent or lease fees for the pet store location', '5000.00'),
('MB00005', 'Pet Care ', 'Used to record expenses for providing pet care services', '25000.00');

-- --------------------------------------------------------

--
-- Table structure for table `monthly_net_report`
--

CREATE TABLE `monthly_net_report` (
  `Monthly_Report_ID` varchar(10) NOT NULL,
  `Month` varchar(50) DEFAULT NULL,
  `Credit_to_Supplier` decimal(10,2) DEFAULT NULL,
  `Debit_from_Customer` decimal(10,2) DEFAULT NULL,
  `Sales_History_ID` varchar(10) DEFAULT NULL,
  `Monthly_Budget_ID` varchar(10) DEFAULT NULL,
  `Profit` decimal(10,2) DEFAULT NULL,
  `Description` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `monthly_net_report`
--

INSERT INTO `monthly_net_report` (`Monthly_Report_ID`, `Month`, `Credit_to_Supplier`, `Debit_from_Customer`, `Sales_History_ID`, `Monthly_Budget_ID`, `Profit`, `Description`) VALUES
('MR00001', 'January', '3000.00', '8000.00', 'SH00001', 'MB00001', '5000.00', 'Get RM 8000 From Customer, Minus Purchase RM 3000, Equal RM 5000. This Month Get RM 5000'),
('MR00002', 'February', '5000.00', '9000.00', 'SH00001', 'MB00002', '4000.00', 'Get RM 9000 From Customer, Minus Purchase RM 5000, Equal RM 4000. This Month Get RM 4000'),
('MR00003', 'March', '3500.00', '7500.00', 'SH00001', 'MB00003', '4000.00', 'Get RM 75000 From Customer, Minus Purchase RM 3500, Equal RM 4000. This Month Get RM 4000'),
('MR00004', 'April', '2000.00', '10000.00', 'SH00001', 'MB00004', '8000.00', 'Get RM 10000 From Customer, Minus Purchase RM 2000, Equal RM 8000. This Month Get RM 8000'),
('MR00005', 'May', '5500.00', '20000.00', 'SH00001', 'MB00005', '14500.00', 'Get RM 20000 From Customer, Minus Purchase RM 5500, Equal RM 14500. This Month Get RM 14500');

-- --------------------------------------------------------

--
-- Table structure for table `payment_history`
--

CREATE TABLE `payment_history` (
  `PaymentHistory_ID` varchar(10) NOT NULL,
  `Customer_ID` varchar(10) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Amount` decimal(10,2) DEFAULT NULL,
  `Payment_Method` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `payment_history`
--

INSERT INTO `payment_history` (`PaymentHistory_ID`, `Customer_ID`, `Date`, `Amount`, `Payment_Method`) VALUES
('PH00001', 'CU00001', '2023-05-20', '3000.00', 'Cash'),
('PH00002', 'CU00002', '2023-05-25', '5000.00', 'Debit Card'),
('PH00003', 'CU00003', '2023-06-06', '4000.00', 'Electronic Transfer'),
('PH00004', 'CU00004', '2023-06-10', '7000.00', 'Credit Card'),
('PH00005', 'CU00005', '2023-06-22', '7500.00', 'Cash');

-- --------------------------------------------------------

--
-- Table structure for table `pet_accessories`
--

CREATE TABLE `pet_accessories` (
  `Accessories_ID` varchar(10) NOT NULL,
  `Category` varchar(20) DEFAULT NULL,
  `Description` text,
  `Color` varchar(20) DEFAULT NULL,
  `Material` varchar(20) DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `Cost_Price` decimal(10,2) DEFAULT NULL,
  `Selling_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_accessories`
--

INSERT INTO `pet_accessories` (`Accessories_ID`, `Category`, `Description`, `Color`, `Material`, `Supplier_ID`, `Cost_Price`, `Selling_Price`) VALUES
('AC00001', 'Bed', 'A Cozy Pet Bed for Sweet Dreams', 'Blue', 'Cotton', 'SU00001', '40.00', '80.00'),
('AC00002', 'Bed', 'A Stylish Leather Collar', 'Black', 'Leather', 'SU00002', '10.00', '20.00'),
('AC00003', 'Bed', 'A Durable Nylon Leash', 'Black', 'Nylon', 'SU00001', '10.00', '20.00'),
('AC00004', 'Bed', 'Sporty Baseball Tank Top', 'Grey', 'Cotton', 'SU00003', '15.00', '30.00'),
('AC00005', 'Bed', 'Playful Plush Toys for Fun', 'Yellow', 'Plush', 'SU00003', '15.00', '30.00');

-- --------------------------------------------------------

--
-- Table structure for table `pet_appointment_record`
--

CREATE TABLE `pet_appointment_record` (
  `Appointment_ID` varchar(10) NOT NULL,
  `Appointment_Date` date DEFAULT NULL,
  `Time` time DEFAULT NULL,
  `Pet_ID` varchar(10) DEFAULT NULL,
  `Service_ID` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_appointment_record`
--

INSERT INTO `pet_appointment_record` (`Appointment_ID`, `Appointment_Date`, `Time`, `Pet_ID`, `Service_ID`) VALUES
('AP00001', '2023-01-08', '10:00:00', 'PID00001', 'SE00001'),
('AP00002', '2023-01-08', '14:30:00', 'PID00002', 'SE00002'),
('AP00003', '2023-01-08', '15:45:00', 'PID00003', 'SE00003'),
('AP00004', '2023-01-08', '11:15:00', 'PID00004', 'SE00004'),
('AP00005', '2023-01-08', '13:00:00', 'PID00005', 'SE00005');

-- --------------------------------------------------------

--
-- Table structure for table `pet_care_service`
--

CREATE TABLE `pet_care_service` (
  `Service_ID` varchar(10) NOT NULL,
  `Service` varchar(50) DEFAULT NULL,
  `Description` text,
  `Duration` varchar(20) DEFAULT NULL,
  `Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_care_service`
--

INSERT INTO `pet_care_service` (`Service_ID`, `Service`, `Description`, `Duration`, `Price`) VALUES
('SE00001', 'Grooming', 'Grooming service for pets. Includes bath, haircut, and nail trimming', '1 hour', '45.00'),
('SE00002', 'Checkup', 'Regular health checkup for pets, including a thorough examination', '45 mins', '35.00'),
('SE00003', 'Vaccination', 'Pet vaccination service to ensure they stay healthy and protected', '1 hour', '50.00'),
('SE00004', 'Training', 'Professional pet training to teach obedience and good behaviour', '1 hour', '55.00'),
('SE00005', 'Boarding', 'Pet boarding service for overnight stays with full care and attention', '2 hours', '60.00');

-- --------------------------------------------------------

--
-- Table structure for table `pet_category`
--

CREATE TABLE `pet_category` (
  `Pet_Category_ID` varchar(10) NOT NULL,
  `Pet_Category_Name` varchar(50) DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `Cost_Price` decimal(10,2) DEFAULT NULL,
  `Selling_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_category`
--

INSERT INTO `pet_category` (`Pet_Category_ID`, `Pet_Category_Name`, `Supplier_ID`, `Cost_Price`, `Selling_Price`) VALUES
('DOG00001', 'Dog - Siberian Husky', 'SU00009', '150.00', '300.00'),
('DOG00002', 'Dog - Poodle', 'SU00009', '200.00', '400.00'),
('DOG00003', 'Dog - French Bulldog', 'SU00009', '400.00', '800.00'),
('DOG00004', 'Dog - Border Collie', 'SU00009', '300.00', '600.00'),
('DOG00005', 'Dog - Golden Retriever', 'SU00009', '250.00', '500.00');

-- --------------------------------------------------------

--
-- Table structure for table `pet_record`
--

CREATE TABLE `pet_record` (
  `Pet_Record_ID` varchar(10) NOT NULL,
  `Pet_ID` varchar(10) DEFAULT NULL,
  `Adoption_Pet_ID` varchar(10) DEFAULT NULL,
  `Lost_Pet_ID` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_record`
--

INSERT INTO `pet_record` (`Pet_Record_ID`, `Pet_ID`, `Adoption_Pet_ID`, `Lost_Pet_ID`) VALUES
('PR00001', 'PID00001', 'AID00001', 'LID00001'),
('PR00002', 'PID00002', 'AID00002', 'LID00002'),
('PR00003', 'PID00003', 'AID00003', 'LID00003'),
('PR00004', 'PID00004', 'AID00004', 'LID00004'),
('PR00005', 'PID00005', 'AID00005', 'LID00005');

-- --------------------------------------------------------

--
-- Table structure for table `pet_treatment_records`
--

CREATE TABLE `pet_treatment_records` (
  `Treatment_Record_ID` varchar(10) NOT NULL,
  `Pet_ID` varchar(10) DEFAULT NULL,
  `Cause` varchar(200) DEFAULT NULL,
  `Drug` varchar(200) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Employee_ID` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `pet_treatment_records`
--

INSERT INTO `pet_treatment_records` (`Treatment_Record_ID`, `Pet_ID`, `Cause`, `Drug`, `Date`, `Employee_ID`) VALUES
('PTR00001', 'PID00001', 'Allergy', 'Diphenhydramine', '2023-08-01', 'EM00001'),
('PTR00002', 'PID00002', 'Diarrhoea', 'Berberine', '2023-08-01', 'EM00002'),
('PTR00003', 'PID00003', 'Malnutrition', 'Vitamin C,B,E', '2023-08-01', 'EM00001'),
('PTR00004', 'PID00004', 'Indigestion', 'Antibiotic', '2023-08-01', 'EM00002'),
('PTR00005', 'PID00005', 'Allergy', 'Diphenhydramine', '2023-08-01', 'EM00001');

-- --------------------------------------------------------

--
-- Table structure for table `purchase_order`
--

CREATE TABLE `purchase_order` (
  `PO_No` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL,
  `Total_Cost_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `purchase_order`
--

INSERT INTO `purchase_order` (`PO_No`, `Date`, `Supplier_ID`, `Product_ID`, `Total_Quantity`, `Total_Cost_Price`) VALUES
('PO00001', '2023-02-08', 'SU00001', 'PD00001', 10, '400.00'),
('PO00002', '2023-02-08', 'SU00002', 'PD00002', 30, '300.00'),
('PO00003', '2023-02-08', 'SU00003', 'PD00003', 40, '400.00'),
('PO00004', '2023-02-08', 'SU00004', 'PD00004', 20, '300.00'),
('PO00005', '2023-02-08', 'SU00001', 'PD00005', 50, '500.00');

-- --------------------------------------------------------

--
-- Table structure for table `sales_history`
--

CREATE TABLE `sales_history` (
  `Sales_History_ID` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Quantity_Sold` int DEFAULT NULL,
  `Total_Selling_Price` decimal(10,2) DEFAULT NULL,
  `Sales_Channel` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `sales_history`
--

INSERT INTO `sales_history` (`Sales_History_ID`, `Date`, `Product_ID`, `Quantity_Sold`, `Total_Selling_Price`, `Sales_Channel`) VALUES
('SH00001', '2023-01-03', 'PD00001', 5, '45.00', 'In-Store'),
('SH00002', '2023-02-05', 'PD00002', 10, '35.00', 'In-Store'),
('SH00003', '2023-03-20', 'PD00003', 20, '60.00', 'Online Store'),
('SH00004', '2023-04-25', 'PD00004', 15, '90.00', 'Social Media Sales'),
('SH00005', '2023-05-10', 'PD00005', 30, '500.00', 'In-Person Sales Event');

-- --------------------------------------------------------

--
-- Table structure for table `shop_event`
--

CREATE TABLE `shop_event` (
  `Event_ID` varchar(10) NOT NULL,
  `Event_Name` varchar(200) DEFAULT NULL,
  `Event_Type` varchar(200) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Location` varchar(100) DEFAULT NULL,
  `Employee_ID` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `shop_event`
--

INSERT INTO `shop_event` (`Event_ID`, `Event_Name`, `Event_Type`, `Date`, `Location`, `Employee_ID`) VALUES
('EV00001', 'Pet Care Workshop', 'Exhibition', '2023-05-20', 'In front of our own store', 'EM00001'),
('EV00002', 'Anniversary Celebration', 'Celebration', '2023-05-25', 'In front of our own store', 'EM00002'),
('EV00003', 'Pet Adoption Day', 'Social and Gathering', '2023-06-06', 'In front of our own store', 'EM00003'),
('EV00004', 'Health Check Day', 'Training and Education', '2023-06-10', 'In front of our own store', 'EM00004'),
('EV00005', 'Pet Social Gathering', 'Social and Gathering', '2023-06-22', 'In front of our own store', 'EM00005');

-- --------------------------------------------------------

--
-- Table structure for table `stock_card`
--

CREATE TABLE `stock_card` (
  `Stock_Card_ID` varchar(10) NOT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Beginning_Quantity` int DEFAULT NULL,
  `In_Quantity` int DEFAULT NULL,
  `Out_Quantity` int DEFAULT NULL,
  `End_Quantity` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `stock_card`
--

INSERT INTO `stock_card` (`Stock_Card_ID`, `Product_ID`, `Beginning_Quantity`, `In_Quantity`, `Out_Quantity`, `End_Quantity`) VALUES
('ST00001', 'PD00001', 10, 10, 5, 15),
('ST00002', 'PD00002', 30, 30, 10, 50),
('ST00003', 'PD00003', 40, 40, 20, 60),
('ST00004', 'PD00004', 20, 20, 10, 30),
('ST00005', 'PD00005', 50, 50, 30, 70);

-- --------------------------------------------------------

--
-- Table structure for table `stock_received`
--

CREATE TABLE `stock_received` (
  `Received_No` varchar(10) NOT NULL,
  `Receive_Date` date DEFAULT NULL,
  `Supplier_Invoice_No` varchar(10) DEFAULT NULL,
  `Supplier_DO_No` varchar(10) DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `PO_No` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `stock_received`
--

INSERT INTO `stock_received` (`Received_No`, `Receive_Date`, `Supplier_Invoice_No`, `Supplier_DO_No`, `Supplier_ID`, `PO_No`, `Product_ID`, `Total_Quantity`) VALUES
('RE00001', '2023-08-03', 'SIN00001', 'SDO00001', 'SU00001', 'PO00001', 'PD00001', 10),
('RE00002', '2023-08-03', 'SIN00002', 'SDO00002', 'SU00002', 'PO00002', 'PD00002', 30),
('RE00003', '2023-08-03', 'SIN00003', 'SDO00003', 'SU00003', 'PO00003', 'PD00003', 40),
('RE00004', '2023-08-03', 'SIN00004', 'SDO00004', 'SU00004', 'PO00004', 'PD00004', 20),
('RE00005', '2023-08-03', 'SIN00005', 'SDO00005', 'SU00005', 'PO00005', 'PD00005', 50);

-- --------------------------------------------------------

--
-- Table structure for table `supplier`
--

CREATE TABLE `supplier` (
  `Supplier_ID` varchar(10) NOT NULL,
  `Supplier_Name` varchar(200) DEFAULT NULL,
  `Phone` varchar(20) DEFAULT NULL,
  `Email` varchar(200) DEFAULT NULL,
  `Item_Supply` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `supplier`
--

INSERT INTO `supplier` (`Supplier_ID`, `Supplier_Name`, `Phone`, `Email`, `Item_Supply`) VALUES
('SU00001', 'Sin Yew Lai Trading Sdn Bhd', '011-454 6455', 'syiewl68@gmail.com', 'Bed, Leash,Bowl'),
('SU00002', 'Jom Jom Pet', '011-332 6654', 'JJ2002@gmail.com', 'Collar'),
('SU00003', 'Sea Park Pet Supplier', '012-541 3236', 'seap68@gmail.com', 'Clothes, Toys'),
('SU00004', 'Pet Lovers Centred', '013-482 0565', 'petlovers@gmail.com', 'Scratcher'),
('SU00005', 'Paws & Whiskers Supply Co.', '014-513 1213', 'pawsnwhiskers22@gmail.com', 'Beds, Toys, Grooming'),
('SU00006', 'Happy Tails Pet Emporium', '012-215 3352', 'happytailspets21@gmail.com', 'Collars, Harnesses, Treats'),
('SU00007', 'Medical Vaccine SdnBhd', '018-520 0662', 'petmedic@gmail.com', 'Vaccine'),
('SU00008', 'Pet Vaccine Sdn Bhd', '012-682 6362', 'petvaccine@gmail.comm', 'Vaccine'),
('SU00009', 'Lovely Pets Sdn Bhd', '015-421 2220', 'happypet@gmail.com', 'Dogs'),
('SU00010', 'Cute Pets Sdn Bhd', '015-225 5600', 'cutepet33@gmail.com', 'Cats');

-- --------------------------------------------------------

--
-- Table structure for table `supplier_do`
--

CREATE TABLE `supplier_do` (
  `Supplier_DO_No` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Receive_Date` date DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `PO_No` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `supplier_do`
--

INSERT INTO `supplier_do` (`Supplier_DO_No`, `Date`, `Receive_Date`, `Supplier_ID`, `PO_No`, `Product_ID`, `Total_Quantity`) VALUES
('SDO00001', '2023-08-03', '2023-08-03', 'SU00001', 'PO00001', 'PD00001', 10),
('SDO00002', '2023-08-03', '2023-08-03', 'SU00002', 'PO00002', 'PD00002', 30),
('SDO00003', '2023-08-03', '2023-08-03', 'SU00003', 'PO00003', 'PD00003', 40),
('SDO00004', '2023-08-03', '2023-08-03', 'SU00004', 'PO00004', 'PD00004', 20),
('SDO00005', '2023-08-03', '2023-08-03', 'SU00005', 'PO00005', 'PD00005', 50);

-- --------------------------------------------------------

--
-- Table structure for table `supplier_invoice`
--

CREATE TABLE `supplier_invoice` (
  `Supplier_Invoice_No` varchar(10) NOT NULL,
  `Date` date DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `PO_No` varchar(10) DEFAULT NULL,
  `Product_ID` varchar(10) DEFAULT NULL,
  `Total_Quantity` int DEFAULT NULL,
  `Total_Cost_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `supplier_invoice`
--

INSERT INTO `supplier_invoice` (`Supplier_Invoice_No`, `Date`, `Supplier_ID`, `PO_No`, `Product_ID`, `Total_Quantity`, `Total_Cost_Price`) VALUES
('SIN00001', '2023-08-02', 'SU00001', 'PO00001', 'PD00001', 10, '400.00'),
('SIN00002', '2023-08-02', 'SU00002', 'PO00002', 'PD00002', 30, '300.00'),
('SIN00003', '2023-08-02', 'SU00003', 'PO00003', 'PD00003', 40, '600.00'),
('SIN00004', '2023-08-02', 'SU00004', 'PO00004', 'PD00004', 20, '800.00'),
('SIN00005', '2023-08-02', 'SU00005', 'PO00005', 'PD00005', 50, '500.00');

-- --------------------------------------------------------

--
-- Table structure for table `vaccine`
--

CREATE TABLE `vaccine` (
  `Vaccine_ID` varchar(10) NOT NULL,
  `Vaccine_Name` varchar(50) DEFAULT NULL,
  `Supplier_ID` varchar(10) DEFAULT NULL,
  `Cost_Price` decimal(10,2) DEFAULT NULL,
  `Selling_Price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `vaccine`
--

INSERT INTO `vaccine` (`Vaccine_ID`, `Vaccine_Name`, `Supplier_ID`, `Cost_Price`, `Selling_Price`) VALUES
('VA00001', 'Leptospirosis', 'SU00007', '60.00', '120.00'),
('VA00002', 'Distemper', 'SU00007', '60.00', '120.00'),
('VA00003', 'Canine influenza', 'SU00007', '60.00', '120.00'),
('VA00004', 'Kennel cough', 'SU00007', '60.00', '120.00'),
('VA00005', 'Feline calicivirus', 'SU00007', '60.00', '120.00');

-- --------------------------------------------------------

--
-- Table structure for table `vaccine_record`
--

CREATE TABLE `vaccine_record` (
  `Vaccine_Record_ID` varchar(10) NOT NULL,
  `Pet_ID` varchar(10) DEFAULT NULL,
  `Vaccine_ID` varchar(10) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Dose` int DEFAULT NULL,
  `Total_Dose` int DEFAULT NULL,
  `Balance_Dose` int DEFAULT NULL,
  `Next_Due_Date` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `vaccine_record`
--

INSERT INTO `vaccine_record` (`Vaccine_Record_ID`, `Pet_ID`, `Vaccine_ID`, `Date`, `Dose`, `Total_Dose`, `Balance_Dose`, `Next_Due_Date`) VALUES
('VR00001', 'PID00001', 'VA00001', '2023-08-01', 1, 2, 1, '2023-10-01'),
('VR00002', 'PID00002', 'VA00002', '2023-08-01', 2, 3, 1, '2023-10-01'),
('VR00003', 'PID00003', 'VA00003', '2023-08-01', 1, 2, 1, '2023-10-01'),
('VR00004', 'PID00004', 'VA00004', '2023-08-01', 2, 3, 1, '2023-10-01'),
('VR00005', 'PID00005', 'VA00004', '2023-08-01', 1, 2, 1, '2023-10-01');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `adoption_record`
--
ALTER TABLE `adoption_record`
  ADD PRIMARY KEY (`Adoption_Pet_ID`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`Customer_ID`);

--
-- Indexes for table `customer_do`
--
ALTER TABLE `customer_do`
  ADD PRIMARY KEY (`DO_No`),
  ADD KEY `Customer_ID` (`Customer_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `customer_feedback`
--
ALTER TABLE `customer_feedback`
  ADD PRIMARY KEY (`Feedback_ID`),
  ADD KEY `Customer_ID` (`Customer_ID`);

--
-- Indexes for table `customer_interest`
--
ALTER TABLE `customer_interest`
  ADD PRIMARY KEY (`Interest_ID`),
  ADD KEY `Customer_ID` (`Customer_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `customer_invoice`
--
ALTER TABLE `customer_invoice`
  ADD PRIMARY KEY (`Invoice_No`),
  ADD KEY `Customer_ID` (`Customer_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `customer_pet`
--
ALTER TABLE `customer_pet`
  ADD PRIMARY KEY (`Pet_ID`),
  ADD KEY `Customer_ID` (`Customer_ID`);

--
-- Indexes for table `employees`
--
ALTER TABLE `employees`
  ADD PRIMARY KEY (`Employee_ID`);

--
-- Indexes for table `employee_training_certification`
--
ALTER TABLE `employee_training_certification`
  ADD PRIMARY KEY (`Training_ID`),
  ADD KEY `Employee_ID` (`Employee_ID`);

--
-- Indexes for table `inventory`
--
ALTER TABLE `inventory`
  ADD PRIMARY KEY (`Product_ID`),
  ADD KEY `Service_ID` (`Service_ID`),
  ADD KEY `Accessories_ID` (`Accessories_ID`),
  ADD KEY `Pet_Category_ID` (`Pet_Category_ID`),
  ADD KEY `Vaccine_ID` (`Vaccine_ID`);

--
-- Indexes for table `lost_and_found_record`
--
ALTER TABLE `lost_and_found_record`
  ADD PRIMARY KEY (`Lost_Pet_ID`);

--
-- Indexes for table `membership`
--
ALTER TABLE `membership`
  ADD PRIMARY KEY (`Membership_ID`),
  ADD KEY `Customer_ID` (`Customer_ID`);

--
-- Indexes for table `monthly_budget`
--
ALTER TABLE `monthly_budget`
  ADD PRIMARY KEY (`Monthly_Budget_ID`);

--
-- Indexes for table `monthly_net_report`
--
ALTER TABLE `monthly_net_report`
  ADD PRIMARY KEY (`Monthly_Report_ID`),
  ADD KEY `Sales_History_ID` (`Sales_History_ID`),
  ADD KEY `Monthly_Budget_ID` (`Monthly_Budget_ID`);

--
-- Indexes for table `payment_history`
--
ALTER TABLE `payment_history`
  ADD PRIMARY KEY (`PaymentHistory_ID`),
  ADD KEY `Customer_ID` (`Customer_ID`);

--
-- Indexes for table `pet_accessories`
--
ALTER TABLE `pet_accessories`
  ADD PRIMARY KEY (`Accessories_ID`),
  ADD KEY `Supplier_ID` (`Supplier_ID`);

--
-- Indexes for table `pet_appointment_record`
--
ALTER TABLE `pet_appointment_record`
  ADD PRIMARY KEY (`Appointment_ID`),
  ADD KEY `Pet_ID` (`Pet_ID`),
  ADD KEY `Service_ID` (`Service_ID`);

--
-- Indexes for table `pet_care_service`
--
ALTER TABLE `pet_care_service`
  ADD PRIMARY KEY (`Service_ID`);

--
-- Indexes for table `pet_category`
--
ALTER TABLE `pet_category`
  ADD PRIMARY KEY (`Pet_Category_ID`),
  ADD KEY `Supplier_ID` (`Supplier_ID`);

--
-- Indexes for table `pet_record`
--
ALTER TABLE `pet_record`
  ADD PRIMARY KEY (`Pet_Record_ID`),
  ADD KEY `Pet_ID` (`Pet_ID`),
  ADD KEY `Adoption_Pet_ID` (`Adoption_Pet_ID`),
  ADD KEY `Lost_Pet_ID` (`Lost_Pet_ID`);

--
-- Indexes for table `pet_treatment_records`
--
ALTER TABLE `pet_treatment_records`
  ADD PRIMARY KEY (`Treatment_Record_ID`),
  ADD KEY `Pet_ID` (`Pet_ID`),
  ADD KEY `Employee_ID` (`Employee_ID`);

--
-- Indexes for table `purchase_order`
--
ALTER TABLE `purchase_order`
  ADD PRIMARY KEY (`PO_No`),
  ADD KEY `Supplier_ID` (`Supplier_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `sales_history`
--
ALTER TABLE `sales_history`
  ADD PRIMARY KEY (`Sales_History_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `shop_event`
--
ALTER TABLE `shop_event`
  ADD PRIMARY KEY (`Event_ID`),
  ADD KEY `Employee_ID` (`Employee_ID`);

--
-- Indexes for table `stock_card`
--
ALTER TABLE `stock_card`
  ADD PRIMARY KEY (`Stock_Card_ID`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `stock_received`
--
ALTER TABLE `stock_received`
  ADD PRIMARY KEY (`Received_No`),
  ADD KEY `Supplier_Invoice_No` (`Supplier_Invoice_No`),
  ADD KEY `Supplier_DO_No` (`Supplier_DO_No`),
  ADD KEY `Supplier_ID` (`Supplier_ID`),
  ADD KEY `PO_No` (`PO_No`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `supplier`
--
ALTER TABLE `supplier`
  ADD PRIMARY KEY (`Supplier_ID`);

--
-- Indexes for table `supplier_do`
--
ALTER TABLE `supplier_do`
  ADD PRIMARY KEY (`Supplier_DO_No`),
  ADD KEY `Supplier_ID` (`Supplier_ID`),
  ADD KEY `PO_No` (`PO_No`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `supplier_invoice`
--
ALTER TABLE `supplier_invoice`
  ADD PRIMARY KEY (`Supplier_Invoice_No`),
  ADD KEY `Supplier_ID` (`Supplier_ID`),
  ADD KEY `PO_No` (`PO_No`),
  ADD KEY `Product_ID` (`Product_ID`);

--
-- Indexes for table `vaccine`
--
ALTER TABLE `vaccine`
  ADD PRIMARY KEY (`Vaccine_ID`),
  ADD KEY `Supplier_ID` (`Supplier_ID`);

--
-- Indexes for table `vaccine_record`
--
ALTER TABLE `vaccine_record`
  ADD PRIMARY KEY (`Vaccine_Record_ID`),
  ADD KEY `Pet_ID` (`Pet_ID`),
  ADD KEY `Vaccine_ID` (`Vaccine_ID`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `customer_do`
--
ALTER TABLE `customer_do`
  ADD CONSTRAINT `customer_do_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`),
  ADD CONSTRAINT `customer_do_ibfk_2` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `customer_feedback`
--
ALTER TABLE `customer_feedback`
  ADD CONSTRAINT `customer_feedback_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`);

--
-- Constraints for table `customer_interest`
--
ALTER TABLE `customer_interest`
  ADD CONSTRAINT `customer_interest_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`),
  ADD CONSTRAINT `customer_interest_ibfk_2` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `customer_invoice`
--
ALTER TABLE `customer_invoice`
  ADD CONSTRAINT `customer_invoice_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`),
  ADD CONSTRAINT `customer_invoice_ibfk_2` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `customer_pet`
--
ALTER TABLE `customer_pet`
  ADD CONSTRAINT `customer_pet_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`);

--
-- Constraints for table `employee_training_certification`
--
ALTER TABLE `employee_training_certification`
  ADD CONSTRAINT `employee_training_certification_ibfk_1` FOREIGN KEY (`Employee_ID`) REFERENCES `employees` (`Employee_ID`);

--
-- Constraints for table `inventory`
--
ALTER TABLE `inventory`
  ADD CONSTRAINT `inventory_ibfk_1` FOREIGN KEY (`Service_ID`) REFERENCES `pet_care_service` (`Service_ID`),
  ADD CONSTRAINT `inventory_ibfk_2` FOREIGN KEY (`Accessories_ID`) REFERENCES `pet_accessories` (`Accessories_ID`),
  ADD CONSTRAINT `inventory_ibfk_3` FOREIGN KEY (`Pet_Category_ID`) REFERENCES `pet_category` (`Pet_Category_ID`),
  ADD CONSTRAINT `inventory_ibfk_4` FOREIGN KEY (`Vaccine_ID`) REFERENCES `vaccine` (`Vaccine_ID`);

--
-- Constraints for table `membership`
--
ALTER TABLE `membership`
  ADD CONSTRAINT `membership_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`);

--
-- Constraints for table `monthly_net_report`
--
ALTER TABLE `monthly_net_report`
  ADD CONSTRAINT `monthly_net_report_ibfk_1` FOREIGN KEY (`Sales_History_ID`) REFERENCES `sales_history` (`Sales_History_ID`),
  ADD CONSTRAINT `monthly_net_report_ibfk_2` FOREIGN KEY (`Monthly_Budget_ID`) REFERENCES `monthly_budget` (`Monthly_Budget_ID`);

--
-- Constraints for table `payment_history`
--
ALTER TABLE `payment_history`
  ADD CONSTRAINT `payment_history_ibfk_1` FOREIGN KEY (`Customer_ID`) REFERENCES `customer` (`Customer_ID`);

--
-- Constraints for table `pet_accessories`
--
ALTER TABLE `pet_accessories`
  ADD CONSTRAINT `pet_accessories_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`);

--
-- Constraints for table `pet_appointment_record`
--
ALTER TABLE `pet_appointment_record`
  ADD CONSTRAINT `pet_appointment_record_ibfk_1` FOREIGN KEY (`Pet_ID`) REFERENCES `customer_pet` (`Pet_ID`),
  ADD CONSTRAINT `pet_appointment_record_ibfk_2` FOREIGN KEY (`Service_ID`) REFERENCES `pet_care_service` (`Service_ID`);

--
-- Constraints for table `pet_category`
--
ALTER TABLE `pet_category`
  ADD CONSTRAINT `pet_category_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`);

--
-- Constraints for table `pet_record`
--
ALTER TABLE `pet_record`
  ADD CONSTRAINT `pet_record_ibfk_1` FOREIGN KEY (`Pet_ID`) REFERENCES `customer_pet` (`Pet_ID`),
  ADD CONSTRAINT `pet_record_ibfk_2` FOREIGN KEY (`Adoption_Pet_ID`) REFERENCES `adoption_record` (`Adoption_Pet_ID`),
  ADD CONSTRAINT `pet_record_ibfk_3` FOREIGN KEY (`Lost_Pet_ID`) REFERENCES `lost_and_found_record` (`Lost_Pet_ID`);

--
-- Constraints for table `pet_treatment_records`
--
ALTER TABLE `pet_treatment_records`
  ADD CONSTRAINT `pet_treatment_records_ibfk_1` FOREIGN KEY (`Pet_ID`) REFERENCES `customer_pet` (`Pet_ID`),
  ADD CONSTRAINT `pet_treatment_records_ibfk_2` FOREIGN KEY (`Employee_ID`) REFERENCES `employees` (`Employee_ID`);

--
-- Constraints for table `purchase_order`
--
ALTER TABLE `purchase_order`
  ADD CONSTRAINT `purchase_order_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`),
  ADD CONSTRAINT `purchase_order_ibfk_2` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `sales_history`
--
ALTER TABLE `sales_history`
  ADD CONSTRAINT `sales_history_ibfk_1` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `shop_event`
--
ALTER TABLE `shop_event`
  ADD CONSTRAINT `shop_event_ibfk_1` FOREIGN KEY (`Employee_ID`) REFERENCES `employees` (`Employee_ID`);

--
-- Constraints for table `stock_card`
--
ALTER TABLE `stock_card`
  ADD CONSTRAINT `stock_card_ibfk_1` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `stock_received`
--
ALTER TABLE `stock_received`
  ADD CONSTRAINT `stock_received_ibfk_1` FOREIGN KEY (`Supplier_Invoice_No`) REFERENCES `supplier_invoice` (`Supplier_Invoice_No`),
  ADD CONSTRAINT `stock_received_ibfk_2` FOREIGN KEY (`Supplier_DO_No`) REFERENCES `supplier_do` (`Supplier_DO_No`),
  ADD CONSTRAINT `stock_received_ibfk_3` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`),
  ADD CONSTRAINT `stock_received_ibfk_4` FOREIGN KEY (`PO_No`) REFERENCES `purchase_order` (`PO_No`),
  ADD CONSTRAINT `stock_received_ibfk_5` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `supplier_do`
--
ALTER TABLE `supplier_do`
  ADD CONSTRAINT `supplier_do_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`),
  ADD CONSTRAINT `supplier_do_ibfk_2` FOREIGN KEY (`PO_No`) REFERENCES `purchase_order` (`PO_No`),
  ADD CONSTRAINT `supplier_do_ibfk_3` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `supplier_invoice`
--
ALTER TABLE `supplier_invoice`
  ADD CONSTRAINT `supplier_invoice_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`),
  ADD CONSTRAINT `supplier_invoice_ibfk_2` FOREIGN KEY (`PO_No`) REFERENCES `purchase_order` (`PO_No`),
  ADD CONSTRAINT `supplier_invoice_ibfk_3` FOREIGN KEY (`Product_ID`) REFERENCES `inventory` (`Product_ID`);

--
-- Constraints for table `vaccine`
--
ALTER TABLE `vaccine`
  ADD CONSTRAINT `vaccine_ibfk_1` FOREIGN KEY (`Supplier_ID`) REFERENCES `supplier` (`Supplier_ID`);

--
-- Constraints for table `vaccine_record`
--
ALTER TABLE `vaccine_record`
  ADD CONSTRAINT `vaccine_record_ibfk_1` FOREIGN KEY (`Pet_ID`) REFERENCES `customer_pet` (`Pet_ID`),
  ADD CONSTRAINT `vaccine_record_ibfk_2` FOREIGN KEY (`Vaccine_ID`) REFERENCES `vaccine` (`Vaccine_ID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
