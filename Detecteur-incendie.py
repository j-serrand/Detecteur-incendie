"""
Nom ......... : Detecteur-incendie.py
Role ........ : Code pour Raspberry Pi s'insérant dans un système complexe de détection d'incendie.
                Permet d'envoyer grâce à un Raspberry Pi des messages sur le serial à une carte Arduino UNO, disposant
                d'un shield GSM.
                La chaîne de caractères à envoyer contient 'alerte', dans le cas ou un incendie a été détecté, le                          séparateur '#' et le numéro de téléphone de la personne à laquelle le SMS doit être envoyé.

                ATTENTION !! : Penser à libérer le serial sur le RaspPi : sudo raspi-config > Advanced Options > Serial

Auteur ...... : J.Serrand
Mail ........ : joris.serrand@rascol.net
Version ..... : V1.0 du 31/03/16
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
"""


import serial #Ajout de la bibliothéque serial pour communiquer par l'UART
import RPi.GPIO as GPIO #Ajout de la bibliothèque Rpi.GPIO gérant le GPIO du RaspPi
import time #Ajout de la bibliothèque time pour les pauses en secondes : time.sleep()

ser = serial.Serial('/dev/ttyACM0', 9600) #Initialisation de la connexion serial avec le UNO à 9600 Bauds

while True:
    ser.write('alerte#xxxxxxxxxx') #Envoi d'un message sur le serial contenant alerte le separateur # et le num de tel ou le SMS doit etre envoyé
    time.sleep(3) # Pause de 3 secondes
