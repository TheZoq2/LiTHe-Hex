# Inledning
## Parter

## Syfte och mål
## Användning
## bakgrundsinformation

# Översikt av systemet
(översiktsbild av systemet)

## Grov beskrivning av systemet
Systemet ska innehålla tre enheter. En Raspberry Pi som agerar som centralenhet, en motorikenhet som sköter hur benen rör sig och en sista enhet för sensorer. Centralenheten är den enhet som kommunicerar med de andra enheterna. 

## Produktkomponenter

## Beroenden till andra system
Det beroende som finns är Raspberry Pi:s blåtandskommunikation som används för att kommunicera med en dator.

## Ingående delsystem
1. Centralenheten (Raspberry Pi)
2. Motorikenhet
3. Sensorenhet

## Avgränsningar


# Delsystem 1...n
## Inledande beskrivning av delsystem 1...n
## Gränssnitt
## Designkrav

# Delsystem centralenhet
Centralenheten ska styra alla andra delsystem i konstruktionen, samt sköta
kommunikation till omvärlden via bland annat Bluetooth. Denna utgörs av en Raspberry
Pi, som är en passande dator då den har inbyggd hårdvara för WiFi och Bluetooth samt
ett operativsystem, som gör att programmering kan ske på en relativt hög nivå.

Krav nr
Original
Centralenheten ska kunna kommunicera med en dator via wifi.
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

# Delsystem motorikenhet

# Delsystem sensorer
Delsystem sensorer är en mikrodator som ska läsa in data från sensorer för att sedan skicka det vidare till huvudstyrenheten. Sensorer är avståndsmätare och möjlighet att läsa av färg. 

Krav nr 
Original
Sensorenheten ska tolka avståndsmätarnas data.
1

Krav nr 
Original
Sensorenheten ska tolka olika färger från färgsensorn. 
1

Krav nr 
Original
Sensorenheten ska kommunicera med centralenheten. Det är tolkad data från sensorer som ska skickas. 
1

# Prestandakrav

# Krav på vidareutveckling

# Ekonomi

# Leveranskrav och delleveranser

# Dokumentation
