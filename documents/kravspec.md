# Inledning
I detta dokument kommer det att framgå vilken funktionalitet som produkten kommer att ha vid leverans. All funktionalitet har strukturerats i olika krav där det blir tydligt huruvida kravet är uppfyllt eller inte. Krav har olika nivåer där nivå 1 är de krav som måste ha uppfyllts vid leverans. Nivå 2 ses som bör-krav och uppfylls i mån av tid. Varje krav kommer att ha följande struktur:
- Krav nr X
- Förändring
- Kravtext
- Kravnivå

## Parter
Projektet har parter som består av beställare/kund Tomas Svensson lektor vid Linköpings tekniska högskola och producent projektgrupp 9 bestående av 7 studenter från D-programmet vid Linköpings tekniska högskola. 

## Syfte och mål
Syftet och målet med projektet är att utveckla en sexbent robot som, själv, kan navigera sig ut ur en labyrint. I labyrinten ska roboten även kunna ta sig över hinder för att komma vidare. 
## Användning
## Bakgrundsinformation

# Översikt av systemet
(översiktsbild av systemet, finns i /documents/images/)

## Grov beskrivning av systemet
Systemet ska innehålla tre enheter. En centralenhet för kommunikation med en dator, en motorikenhet som sköter hur benen rör sig och en sista enhet för sensorer. Centralenheten är även den enhet som tar beslut och kommunicerar med de andra enheterna. 

Krav nr
Original
Roboten ska vara autonom 
1

Krav nr
Original
Det ska finnas en knapp där man startar roboten i tävlingen. 
1

Krav nr
Original
Roboten ska gå att styra med dator via Wi-Fislänk (Fram, bakåt, rotera, ...). 
1

Krav nr
Original
Robotens sensordata ska gå att läsa med dator via Wi-Fi. 
1

Krav nr
Original
Robotens styrbeslut ska gå att läsa med dator via Wi-Fi. 
1


## Produktkomponenter
I leveransen ska det ingå en autonom sexbent robot med tillhörande GUI som kan användas för att styra roboten manuellt. Teknisk dokumentation och demonstration ingår även. 

## Beroenden till andra system
Det beroende som finns är centralenhetens Wi-Fi-kommunikation som används för att kommunicera med en dator.

## Ingående delsystem
1. Centralenheten
2. Motorikenhet
3. Sensorenhet

## Avgränsningar


# Delsystem 1...n
## Inledande beskrivning av delsystem 1...n
## Gränssnitt
## Designkrav

# Delsystem centralenhet
Centralenheten ska styra alla andra delsystem i konstruktionen, samt sköta
kommunikation till omvärlden via bland annat Wi-Fi. Denna utgörs av en Raspberry
Pi, som är en passande dator då den har inbyggd hårdvara för WiFi och Wi-Fi samt
ett operativsystem, som gör att programmering kan ske på en relativt hög nivå.

Krav nr
Original
Centralenheten ska kunna kommunicera med en dator via Wi-Fi.
1

Krav nr
Original
Centralenheten ska kunna ta emot och behandla data från sensorenheten.
1

Krav nr
Original
Centralenheten ska kunna ta emot, behandla och skicka information till motorikenheten. 
1

Krav nr
Original
Centralenheten ska kunna hålla koll på positionen i labyrinten med hjälp av sensorerna.
1

Krav nr
Original
Roboten ska kunna undvika att kollidera med väggar i banan. 
1

# Delsystem GUI
Gränssnittet mot datorn ska vara i form av en webbsida, som centralenheten 
tillhandahåller via en Wi-Fi hotspot.

Krav nr
Original
Gränssnittet ska förmedla information som ges av sensorerna och annan information
relevant för felsökning.
1

Krav nr
Original
Det manuella läget ska kunna styras från gränssnittet med hjälp av en joysticḱ.
1

Krav nr
Original
Det autonoma läget ska initieras och bevakas från gränssnittet.
1

# Delsystem motorikenhet
(BILD)

Krav nr 
Original
Roboten ska kunna gå framåt, bakåt, ?höger och vänster?, samt rotera.
1

Krav nr 
Original
Roboten ska kunna kliva över hinder med (INSERT HÖJD HÄR) 
1?


# Delsystem sensorer
Delsystem sensorer är en mikrodator som ska läsa in data från sensorer för att sedan skicka det vidare till centralenheten. Sensorer är avståndsmätare och möjlighet att läsa av färg (eventuellt en kamera). 

Krav nr 
Original
Sensorenheten ska kunna tolka avståndsmätarnas data.
1

Krav nr 
Original
Sensorenheten ska kunna tolka olika färger från färgsensorn. 
1

Krav nr 
Original
Sensorenheten ska kunna kommunicera med centralenheten. Det är tolkad data från sensorer som ska skickas. 
1

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


