CREATE TABLE "Athlete"(
	licenseid INT NOT NULL,
	name VARCHAR NOT NULL,
	gender VARCHAR,
	nationality VARCHAR,
	age INT,
	club VARCHAR,
	CONSTRAINT PK_Athlete PRIMARY KEY (licenseid)
);

CREATE TABLE "Series"(
	seriesid VARCHAR NOT NULL,
	participantrow INT NOT NULL,
	finalscore REAL,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL,
	CONSTRAINT PK_Series PRIMARY KEY (seriesid)
);

CREATE TABLE "Competition"(
	competitionid VARCHAR NOT NULL,
	name VARCHAR,
	location VARCHAR NOT NULL,
	date DATE NOT NULL,
	category VARCHAR NOT NULL,
	CONSTRAINT PK_Competition PRIMARY KEY (competitionid)
);

CREATE TABLE "Coordinates"(
	coordinatesid serial,
	coordinatex REAL NOT NULL,
	coordinatey REAL NOT NULL,
	score VARCHAR NOT NULL,
	seriesid VARCHAR NOT NULL,
	CONSTRAINT PK_Coordinates PRIMARY KEY (coordinatesid)
);

CREATE TABLE "Rank"(
	place INT NOT NULL,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL
);

ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" (licenseid) ON DELETE NO ACTION ON UPDATE NO ACTION;			
ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;			

ALTER TABLE "Coordinates"
ADD CONSTRAINT FK_CoordinatesSeriesID FOREIGN KEY (seriesid) REFERENCES "Series" (seriesid) ON DELETE NO ACTION ON UPDATE NO ACTION;	

ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" (licenseid) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;
