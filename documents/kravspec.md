# Inledning
I detta dokument kommer det att framgå vilken funktionalitet som produkten kommer att ha vid leverans. All funktionalitet har strukturerats i olika krav där det blir tydligt huruvida kravet är uppfyllt eller inte. Krav har olika nivåer där nivå 1 är de krav som måste ha uppfyllts vid leverans. Nivå 2 ses som bör-krav och uppfylls i mån av tid. Varje krav kommer att ha följande struktur:
- Krav nr X
- Förändring
- Kravtext
- Kravnivå

## Parter
Projektets parter utgörs av beställare/kund Tomas Svensson, lektor vid Linköpings tekniska högskola, samt producent projektgrupp 9, bestående av 7 studenter från D-programmet vid Linköpings tekniska högskola. 

## Syfte och mål
Syftet med detta projekt är att bredda våra kunskaper inom konstruktion med
mikrodatorer och arbete enligt projektmodellen LIPS.

Målet med projektet är att ha utvecklat en sexbent robot som själv kan navigera sig ut ur en labyrint, samt styras manuellt. 

## Användning

## Bakgrundsinformation
För regler om uppbyggnad om labyrinten roboten är tänkt att ta sig igenom, se
Bilaga A: Banregler.

# Översikt av systemet
Systemet ska innehålla tre enheter. En centralenhet för kommunikation med en dator, en motorikenhet som sköter hur benen rör sig samt en sista enhet för sensorer. Centralenheten är även den enhet som tar beslut och kommunicerar med de andra enheterna.

(översiktsbild av systemet, finns i /documents/images/)

## Grov beskrivning av produkten 
Produkten är en sexbent robot som känner av sin omgivning, och kan gå autonomt
genom en bana samt styras manuellt.

## Produktkomponenter
I leveransen ska det ingå en autonom sexbent robot med tillhörande GUI som kan användas för att styra roboten manuellt. Teknisk dokumentation och demonstration ingår även. 

## Beroenden till andra system
Det beroende som finns är centralenhetens WiFi-kommunikation som används för att kommunicera med en dator.

## Ingående delsystem
1. Centralenheten
2. Motorikenheten
3. Sensorenheten

## Avgränsningar
Roboten behöver inte klara mer avancerade former på labyrinten den beskriven i Bilaga A: Banregler.

## Generella krav på hela systemet

- Krav nr
Original
Roboten ska ha ett autonomt läge.
1

- Krav nr
Original
Det ska finnas en knapp där man startar roboten. 
1

- Krav nr
Original
Roboten ska kunna styras med dator via WiFi-länk. 
1

- Krav nr
Original
Robotens sensordata ska gå att läsa med dator via WiFi. 
1

- Krav nr
Original
Robotens styrbeslut ska gå att läsa med dator via WiFi. 
1

- Krav nr Original Roboten ska kunna gå över hinder beskrivet i Bilaga A: Banregler.  2

- Krav nr Original Roboten ska inte kollidera med väggarna i labyrinten.  1

# Delsystem centralenhet
Centralenheten ska styra alla andra delsystem i konstruktionen, samt sköta
kommunikation till omvärlden via bland annat WiFi. Denna utgörs av en Raspberry
Pi, som är en passande dator då den har inbyggd hårdvara för WiFi samt
ett operativsystem, som gör att programmering kan ske på en relativt hög nivå.

- Krav nr Original Centralenheten ska kunna kommunicera med en dator via WiFi.  1

- Krav nr Original Centralenheten ska kunna ta emot och behandla data från sensorenheten.  1

- Krav nr Original Centralenheten ska kunna ta emot, behandla och skicka information till motorikenheten.  1

- Krav nr Original Centralenheten ska kunna hålla koll på positionen i labyrinten med hjälp av sensorerna.  1
 
# Delsystem GUI
Gränssnittet mot datorn ska vara i form av en webbsida, som centralenheten 
tillhandahåller via en WiFi-hotspot.

- Krav nr Original Gränssnittet ska förmedla information som ges av sensorerna och annan information relevant för felsökning.  1
 
- Krav nr Original Det manuella läget ska kunna styras från gränssnittet med hjälp av en joysticḱ.  1
 
- Krav nr Original Det autonoma läget ska initieras och bevakas från gränssnittet.  1

# Delsystem motorikenhet
(BILD)

- Krav nr Original Roboten ska kunna gå framåt, bakåt, ?höger och vänster?, samt rotera.  1
 
- Krav nr Original Roboten ska kunna kliva över hinder med (INSERT HÖJD HÄR) 1?

- Krav nr Original Roboten ska kunna röra sig framåt och bakåt samt rotera.  1

- Krav nr Original Roboten ska kunna röra sig åt höger och vänster.  2

# Delsystem sensorer
Delsystem sensorer är en mikrodator som ska läsa in data från sensorer för att sedan skicka det vidare till centralenheten. Sensorer är avståndsmätare och möjlighet att läsa av färg (eventuellt en kamera). 

- Krav nr Original Sensorenheten ska kunna tolka avståndsmätarnas data.  1
 
- Krav nr Original Sensorenheten ska kunna tolka olika färger från färgsensorn.  1
 
- Krav nr Original Sensorenheten ska kunna kommunicera med centralenheten. Det är tolkad data från sensorer som ska skickas.  1

- Krav nr Original Roboten ska kunna röra sig åt höger och vänster.  2

#Motorikkontrollern
Benkontrollerns syfte är att ta data om vilket håll roboten ska gå från
styrenheten och styra benen enligt de instruktionerna. Den ska 
bestå av en AVR processor som tar kommandon från styrdatorn och skickar servopositioner 
till de individuella servona.


##Designkrav
| krav nr | ? | beskrivning                                                                                                                  |
| n       | o | Motorikkontrollern ska kunna ta steg  i olika riktningar på kommando från centralenhet                                       |
| n       | o | Motorikkontrollern ska kunna skicka data om höjden på underlaget till centralenheten                                         |
| n       | o | Motorikkontrollern ska klara av 2 olika gång lägen. Ett där den går snabbt och ett där den går långsamt med högre prescition |






# Prestandakrav
Roboten ska kunna gå framåt, bakåt, höger och vänster, samt rotera. Den ska kunna
upptäcka och kliva över hinder med (INSERT HÖJD HÄR). Den ska kunna navigera
utan att kollidera med väggar.

Krav nr
Original
Roboten ska kunna gå någorlunda i mitten och rakt farm i banan. 
1

# Krav på vidareutveckling
Roboten ska kunna utvecklas vidare för att få en mer avancerad styrning av benen. 
Exempelvis skall den kunna gå snabbare och jämnare. Koden och hårdvaran ska också vara
konstruerad på ett sådant sätt att det ska gå att programmera roboten för
kartläggning av ett utrymme.

# Ekonomi
Vid projektets slutförande skall 960 timmars arbetstid ha nedlagts.

# Leveranskrav och delleveranser
Delleveranser är leverans av projektplan, leverans av designspecifikation 
och slutleverans. Slutleveransen består av en presentation av projektet, 
demonstration av roboten i autonomnt och manuellt läge i form av en tävling,
samt överlämning av kod, hårdvara och dokumentation.

# Dokumentation
Tidplan 
Systemskiss 
Projektplan
Teknisk dokumentation 
Användarhandledning 


