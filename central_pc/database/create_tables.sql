CREATE TABLE "Athlete"(
	"Licenca" INT NOT NULL,
	"Nome" VARCHAR NOT NULL,
	"Clube" VARCHAR NOT NULL,
	"Disciplina" VARCHAR,
	"Escalao" VARCHAR,
	"Data de Nascimento" DATE,
	"Pais" VARCHAR,
	"Observacoes" VARCHAR,
	CONSTRAINT PK_Athlete PRIMARY KEY ("Licenca")
);

CREATE TABLE temp AS TABLE "Athlete" WITH NO DATA;

CREATE TABLE "Competition"(
	competitionid VARCHAR NOT NULL,
	"Nome" VARCHAR,
	"Cidade" VARCHAR NOT NULL,
	"Data" DATE NOT NULL,
	"Categoria" VARCHAR NOT NULL,
	CONSTRAINT PK_Competition PRIMARY KEY (competitionid)
);

CREATE TABLE "Series"(
	seriesid VARCHAR NOT NULL,
	participantrow INT NOT NULL,
	finalscore REAL,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL,
	isFinal BOOLEAN NOT NULL,
	CONSTRAINT PK_Series PRIMARY KEY (seriesid)
);



CREATE TABLE "Coordinates"(
	coordinatesid VARCHAR NOT NULL,
	coordinatex REAL NOT NULL,
	coordinatey REAL NOT NULL,
	score REAL NOT NULL,
	seriesid VARCHAR NOT NULL,
	CONSTRAINT PK_Coordinates PRIMARY KEY (coordinatesid)
);

CREATE TABLE "Rank"(
	rankid VARCHAR NOT NULL,
	place INT NOT NULL,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL,
	CONSTRAINT PK_Rank PRIMARY KEY (rankid)
);

ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" ("Licenca") ON DELETE NO ACTION ON UPDATE NO ACTION;			
ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;			

ALTER TABLE "Coordinates"
ADD CONSTRAINT FK_CoordinatesSeriesID FOREIGN KEY (seriesid) REFERENCES "Series" (seriesid) ON DELETE NO ACTION ON UPDATE NO ACTION;	

ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" ("Licenca") ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;



WHERE "Competition".competitionid = 

SELECT "Athlete"."Licenca", "Series".finalscore 
FROM "Athlete"
JOIN "Series" ON "Series".licenseid = "Athlete"."Licenca"
WHERE "Series".competitionid = 'BRAGA_12.02.2021_P'
ORDER BY "Series".finalscore DESC;

SELECT "Coordinates".coordinatex, "Coordinates".coordinatey, "Coordinates".score
FROM "Coordinates"
WHERE "Coordinates".seriesid = '3000000_BRAGA_12.02.2021_P';

COPY (SELECT "Coordinates".coordinatex, "Coordinates".coordinatey, "Coordinates".score
FROM "Coordinates"
WHERE "Coordinates".seriesid = '3000000_BRAGA_12.02.2021_P') TO '/home/ines/teste1.csv'
WITH (FORMAT csv, HEADER, DELIMITER ',',ENCODING 'ISO-8859-1');