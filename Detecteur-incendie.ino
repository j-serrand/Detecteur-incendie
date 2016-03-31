/***********************************************************************************************************************
Nom ......... : Detecteur-incendie.ino
Role ........ : Code pour Arduino UNO s'insérant dans un système complexe de détection d'incendie.
                Ce code permet de recevoir sur un Arduino UNO des chaînes de caractères venant d'une
                platine Raspberry Pi par l'UART ,du type : 'Alerte#066822XXXX' désignant le souhait
                de prévenir l'utilisateur dont le numéro de téléphone suit, de l'apparition d'un incendie
                par l'envoi d'un sms.
                L'arduino UNO est connecté à un shield Arduino GSM.

                Pour plus de conviavialité un affichage du déroulement du programme se fait sur un écran lcd i2c
                de chez Seed : Grove-LCD RGB Backlight, penser à télécharger et installer la bibliothèque :
                https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight

Auteur ...... : J.Serrand
Mail ........ : joris.serrand@rascol.net
Version ..... : V1.0 du 30/03/16
Licence ..... : Copyright (C) 2016  Joris SERRAND

                This program is free software: you can redistribute it and/or modify
                it under the terms of the GNU General Public License as published by
                the Free Software Foundation, either version 3 of the License, or
                (at your option) any later version.

                This program is distributed in the hope that it will be useful,
                but WITHOUT ANY WARRANTY; without even the implied warranty of
                MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
                GNU General Public License for more details.

                You should have received a copy of the GNU General Public License
                along with this program.  If not, see <http://www.gnu.org/licenses/>

Compilation . : Avec l'IDE Arduino
***********************************************************************************************************************/

#include <Arduino.h> //A commenter bibli pour compilation avec PlatformIO sous éditeur Atom
#include <Wire.h> // Bibli pour utiliser la liaison i2c (pour lcd Grove)
#include <GSM.h> //Bibli pour utilisation des fonctions GSM avec l'Arduino GSM shield
#include "rgb_lcd.h" //Bibli pour utiliser le lcd i2C Grove RGB

//Définitions pour le lcd
rgb_lcd lcd; //Déclaration de l'objet rgb_lcd
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//Définitions pour l'Arduino GSM Shield
#define PINNUMBER "XXXX" //Définition du code pin de la carte sim /!\ Entre guillemets !
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;//Construction d'un objet sms à partir du constructeur GSM_SMS

/**********************************************************************************************************************/

void setup()
{
    //Initialisation de l'UART
    Serial.begin(9600);
    while (!Serial)
    {
    ; // wait for serial port to connect. Needed for native USB
    }

    //Initialisations pour le lcd
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);


    //Initialisations pour l'Arduino GSM shield
    boolean notConnected = true; //Déclaration d'un booleen pour etat connexion réseau
    while(notConnected)
    {
      if(gsmAccess.begin(PINNUMBER)==GSM_READY)
        notConnected = false;
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("GSM non connecte");
        delay(1000);
      }
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GSM connecte !");
    delay(1000);

}

/**********************************************************************************************************************/

void loop()
{
  String NumeroString, DemandeRaspPi; //Déclaration de 2 chaines contenant les messages de reception et de retour
  NumeroString=String();//Initialisation avec rien
  DemandeRaspPi=String();//Initialisation avec rien
  char NumeroChar[11];//Déclaration d'un tableau de char de 10 cases qui contiendra le num a utiliser pour le sms
  char txtMsg[100]="DETECTION INCENDIE ! \nLe detecteur d'incendie a ete declenche dans votre logement";

  if(Serial.available() > 0)//Attente de la réception d'un message venant du RaspPi sur le serial
      {
        DemandeRaspPi = Serial.readStringUntil('#'); //Lecture du message jusqu'au caractère '#'
        NumeroString = Serial.readString(); //Lecture du num dans la suite du message
        NumeroString.toCharArray(NumeroChar,11); //Stockage du num dans NumeroChar
      }

  if (DemandeRaspPi=="alerte") //Si le message = "alerte" envoi d'un sms pour incendie
      {
        lcd.clear(); //Effacement du lcd
        lcd.setCursor(0,0); //On ecrit en haut à gauche
        lcd.print("Alerte sms :");
        lcd.setCursor(0,1);
        lcd.print(NumeroChar); //On affiche le num auquel le message va etre envoyé
        delay(1000);

        sms.beginSMS(NumeroChar);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("begin SMS : OK"); //Le numero a bien été saisi
        delay(1000);

        sms.print(txtMsg);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("print SMS : OK"); //Le contenu du message a bien été saisi
        delay(1000);

        sms.endSMS();//Envoi du message

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Message envoye !"); // Le message a bien été envoyé
        delay(1000);

        DemandeRaspPi=String();//Reset de la variable

      }
}
