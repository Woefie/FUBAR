# Tegenwindwagen
Realtime Project Windwagen

Code is geschreven voor de ESP-WROOM-32.
Gebruik is gemaakt van het ESP-IDF framework (https://docs.espressif.com/projects/esp-idf/en/latest/get-started/)
FREE_RTOS framework voor de tasks (https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/freertos.html

#GPIO aansluitingen:
3.3V:
Wind direction sensor

5V:
Wind speed sensor

G4 - Pulse input wind speed sensor

G16 - Step PITCH stepper controller

G17 - Direction PITCH stepper controller

G18 - Step YAW stepper controller

G19 - Direction YAW stepper controller

G34 - ADC input wind direction sensor

#Opbouw code
Code is verdeeld in de volgende tasks:
- dirSensor: Verkrijg de positie van de windrichting sensor en reken de digitale waarde om naar positie in graden.
- speedSensor: Verkrijg de windsnelheid in pulsen en reken dit om naar m/s.
- rotorSpeed: Verkrijg het RPM van de rotor. Is alvast toegevoegd, omdat nog niet bekend is hoe de data hiervoor ontvangen wordt is de implementatie verder leeg. Wel wordt een potmeter uitgelezen om de rotor RPM te simuleren voor testen.
- controller: Ontvangt de waardes van de sensoren en rekent deze om naar stappen om stepper motoren aan te sturen. Tevens is een PID controller geimplementeerd voor het berekenen van de pitch hoek.
- yawController: Ontvangt van de controller het aantal steps dat gezet moet worden.
- pitchController: Ontvangt van de controller de angle waar de pitch op moet staan en stuurt vervolgens de stepper aan om de pitch aan te passen.

#Aanpasbare instellingen
In de file controller.h kunnen de volgende instellingen aangepast worden: 
- De periodes waarin de taken uitgevoerd worden. 
- Het aantal graden waarin de turbine zich moet bevinden. Wanneer de punt op de windvaan naar achter wijst, bevindt deze zone zich tussen 142 en 148 graden.
- Hoeveel graden 1 stap op de stepper is voor de yaw. Indien andere stepper gebruikt wordt, pas dit aan.
- Hoeveel rotaties de stepper moet maken, om de yaw één rotatie te laten doen.
- De waarde die gebruikt wordt om de ADC waarde van de windvaan om te rekenen naar het aantal graden.
- Hoeveel metingen gedaan worden voor het verkrijgen van één accuratere meting.
- Instellingen voor de PID controller.
- Maximale RPM van de rotor.
- Minimale en maximale hoek van de rotor.
- Hoeveel graden 1 stap op de stepper is voor de pitch. Indien andere stepper gebruikt wordt, pas dit aan.
- Hoeveel rotaties de stepper moet maken, om de pitch één rotatie te laten doen.
