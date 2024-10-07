/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.groupno2_petshopprogram;
import java.util.LinkedList;
import javax.swing.JOptionPane;

/**
 *
 * @author HP
 */
public class GroupNo2_PetShopProgram {
    
//Stack<String>s = new Stack<String>();
static LinkedList<String> Item1 = new LinkedList<>();
static LinkedList<Double> Price1 = new LinkedList<>();
static LinkedList<Integer> Quantity1 = new LinkedList<>();

static LinkedList<String> Item2 = new LinkedList<>();
static LinkedList<Double> Price2 = new LinkedList<>();
static LinkedList<Integer> Quantity2 = new LinkedList<>();

    public static void main(String[] args) 
    {   
        WelcomePage();
        Menu();
        AboutUs();
        BuyPets();
        BuyPetsFood();
        ConfirmPayment();
    }

        
    public static void WelcomePage() 
    {
        JOptionPane.showMessageDialog(null,
                String.format("                       / \\ _ / \\   (      . - \" - . \n"+
                                    "                      (  ^ . ^  )   )  / | ^  ^ | \\ \n"+
                                    "                          \\ \" /    (   { / (_0_) \\ } \n"+
                                    "                      (   |   |  )       _ /  ^  \\ _ \n"+
                                    "                    (__d b__)    (  /  / ^ \\  \\ )\n\n"+
                                    "(*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~(\n"+
                                    ")*   !          !    ^ ^ ^     [  *  *  *  \\  {  ~  ~ ~  \\  \\       /   *)\n"+
                                    "(*   !          ! /             \\ [            ]  {             }   \\    /     *(\n"+
                                    ")*   !$$$$$! |@@@@| [  *  *  * /   {  ~  ~ ~  /     <       *)\n"+
                                    "(*   !          ! |             | [               {                    >      *(\n"+
                                    ")*   !          ! |             | [               {                    <      *)\n"+
                                    "(*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~(\n\n"+
                                    "WELCOME TO HAPPY PETS SHOP!^^"));
    }
     public static void Menu() 
     {
        while (true) 
        {
            String Menu = JOptionPane.showInputDialog(
                    "Please select the number:\n"+
                    "1. About Us\n"+
                    "2. Buy Pets\n"+
                    "3. Buy Pets Food\n"+
                    "4. Confirm Payment\n"+
                    "5. Exit");

            int selection = Integer.parseInt(Menu);

            switch (selection) 
            {
                case 1: AboutUs();break;
                case 2: BuyPets();break;
                case 3: BuyPetsFood();break;
                case 4: ConfirmPayment();break;
                case 5: JOptionPane.showMessageDialog(null,
                String.format(" Thank you for visit Happy Pets Shop"));
                    System.exit(0); break;
                default:
                    JOptionPane.showMessageDialog(null,
                String.format(" Invalid Selection"));
            }
        }
    }

    public static void AboutUs() 
    {
        JOptionPane.showMessageDialog(null,
                String.format("############################################\n"+
                                    "#                 ABOUT US                 #\n"+
                                    "############################################\n"+
                                    "# Company name    | Happy Pets Shop        #\n"+
                                    "#-----------------|------------------------#\n"+
                                    "# Registration No | 123456789101(1234567A) #\n"+
                                    "#-----------------|------------------------#\n"+
                                    "# Representative  | Su Zhi Lun             #\n"+
                                    "#                 | Wong Jie Ying          #\n"+
                                    "#                 | Beatrice Leong Zhi Xin #\n"+
                                    "#                 | Angel Yong Zu Er       #\n"+
                                    "#-----------------|------------------------#\n"+
                                    "# Date of         | 19/08/2023             #\n"+
                                    "# establishment   | 19 August 2023         #\n"+
                                    "#-----------------|------------------------#\n"+
                                    "$ Business        | Pets                   #\n"+
                                    "# description     | Sales                  #\n"+
                                    "#-----------------|------------------------#\n"+
                                    "# Address         | Kajang, Malaysia       #\n"+
                                    "############################################\n"));
    }

    public static void BuyPets() 
    {
        String Pets [] = {"Cat", "Dog", "Fish", "Rabbit","Bird"};
        double PetsPrice [] = {1800.00, 1500.00, 500.00, 1000.00, 1200.00};
        int PetsQuantity;
        
        String PetsMenu = JOptionPane.showInputDialog(
                    "Please select the number:\n"+
                    "1. Cat (RM 1,800.00)\n"+
                    "2. Dog (RM 1,500.00)\n"+
                    "3. Fish (RM 500.00)\n"+
                    "4. Rabbit (RM 1,000.00)\n"+ 
                    "5. Bird (RM 1,200.00)\n");

        int PetsSelection = Integer.parseInt(PetsMenu);
        
        if (PetsSelection >= 1 && PetsSelection <= 5) 
        {
            String SelectedPets = Pets[PetsSelection - 1];
            double SelectedPetsPrice = PetsPrice[PetsSelection - 1];
            
            String InputPetsQuantity = JOptionPane.showInputDialog("Enter the quantity of pets: ");
            PetsQuantity = Integer.parseInt(InputPetsQuantity);           
            
            Item1.push(SelectedPets);
            Price1.push(SelectedPetsPrice);
            Quantity1.push(PetsQuantity);            
        }  
        else 
        {
            JOptionPane.showMessageDialog(null,
                String.format("Invalid Selection"));
        }
    }
    public static void BuyPetsFood()
    {
        String PetsFood [] = {"Cat Food", "Dog Food", "Fish Food", "Rabbit Food","Bird Food"};
        double PetsFoodPrice [] = {85.00, 80.00, 50.00, 65.00, 55.00};
        int PetsFoodQuantity;
        
        String PetsFoodMenu = JOptionPane.showInputDialog(
                    "Please select the number:\n"+
                    "1. Cat Food (RM 85.00)\n"+
                    "2. Dog Food (RM 80.00)\n"+
                    "3. Fish Food (RM 50.00)\n"+
                    "4. Rabbit Food (RM 65.00)\n"+ 
                    "5. Bird Food (RM 55.00)\n");

        int PetsFoodSelection = Integer.parseInt(PetsFoodMenu);
        
        if (PetsFoodSelection >= 1 && PetsFoodSelection <= 5) 
        {
            String SelectedPetsFood = PetsFood[PetsFoodSelection - 1];
            double SelectedPetsFoodPrice = PetsFoodPrice[PetsFoodSelection - 1];
            
            String InputPetsFoodQuantity = JOptionPane.showInputDialog("Enter the quantity of pets food: ");
            PetsFoodQuantity = Integer.parseInt(InputPetsFoodQuantity);
            
            Item2.push(SelectedPetsFood);
            Price2.push(SelectedPetsFoodPrice);
            Quantity2.push(PetsFoodQuantity);
        }  
        else 
        {
            JOptionPane.showMessageDialog(null,
             String.format("Invalid Selection"));
        }  
    }
    public static void ConfirmPayment()
    {
        double Total = 0;
        double SST;
        double SubTotal;
        StringBuilder Receipt = new StringBuilder("Item               \t\tPrice (RM)          \tQuantity          \tTotal (RM)\n");

        for (int i = 0; i < Item1.size(); i++) 
        {
            double Total1 = Price1.get(i) * Quantity1.get(i);
            Total += Total1;
            Receipt.append(Item1.get(i)).append("                  \t").append(
                    String.format("%.2f", Price1.get(i))).append("               \t").append(Quantity1.get(i)).append("                 \t").append(
                    String.format("%.2f", Total1)).append("\n\n");
        }

        for (int i = 0; i < Item2.size(); i++) 
        {
            double Total2 = Price2.get(i) * Quantity2.get(i);
            Total += Total2;
            Receipt.append(Item2.get(i)).append("           \t").append(
                    String.format("%.2f", Price2.get(i))).append("                 \t").append(Quantity2.get(i)).append("                    \t").append(
                    String.format("%.2f", Total2)).append("\n\n");
        }

        if (Item1.isEmpty() && Item2.isEmpty()) {
            Receipt.append("No items purchased.\n");
        } 
        else 
        {
            Receipt.append("Total:   \t\t\t\t\t").append(String.format("%.2f", Total)).append("\n");

            SST = Total * 0.06;
            SubTotal = SST + Total;

            Receipt.append("SST TAX (6%):   \t\t\t\t\t").append(String.format("%.2f", SST)).append("\n");
            Receipt.append("Sub Total:   \t\t\t\t\t").append(String.format("%.2f", SubTotal));
        }

        JOptionPane.showMessageDialog(null, Receipt.toString());
    }
}