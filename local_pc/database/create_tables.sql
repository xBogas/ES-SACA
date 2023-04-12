
	
CREATE TABLE "Athlete"(
	licenseid INT NOT NULL,
	name VARCHAR,
	gender VARCHAR,
	nationality VARCHAR,
	age INT,
	club VARCHAR,
	CONSTRAINT PK_Athlete PRIMARY KEY (licenseid)
);

CREATE TABLE "Series"(
	seriesid VARCHAR NOT NULL,
	participantrow INT,
	category VARCHAR,
	finalscore REAL,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL,
	ispractice BOOL,
	CONSTRAINT PK_Series PRIMARY KEY (seriesid)
);

CREATE TABLE "Competition"(
	competitionid VARCHAR NOT NULL,
	name VARCHAR,
	location VARCHAR,
	date DATE,
	CONSTRAINT PK_Competition PRIMARY KEY (competitionid)
);

CREATE TABLE "Remark"(
	remark VARCHAR,
	seriesid VARCHAR
);

CREATE TABLE "Coordinates"(
	coordinatesid VARCHAR NOT NULL,
	coordinatex REAL,
	coordinatey REAL,
	score VARCHAR,
	seriesid VARCHAR,
	CONSTRAINT PK_Coordinates PRIMARY KEY (coordinatesid)
);


CREATE TABLE "Rank"(
	place INT,
	licenseid INT NOT NULL,
	competitionid VARCHAR NOT NULL
);

ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" (licenseid) ON DELETE NO ACTION ON UPDATE NO ACTION;			
ALTER TABLE "Series"
ADD CONSTRAINT FK_SeriesCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;			

ALTER TABLE "Remark"
ADD CONSTRAINT FK_RemarkSeriesID FOREIGN KEY (seriesid) REFERENCES "Series" (seriesid) ON DELETE NO ACTION ON UPDATE NO ACTION;

ALTER TABLE "Coordinates"
ADD CONSTRAINT FK_CoordinatesSeriesID FOREIGN KEY (seriesid) REFERENCES "Series" (seriesid) ON DELETE NO ACTION ON UPDATE NO ACTION;	


ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankAthleteID FOREIGN KEY (licenseid) REFERENCES "Athlete" (licenseid) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE "Rank"
ADD CONSTRAINT FK_RankCompetitionID FOREIGN KEY (competitionid) REFERENCES "Competition" (competitionid) ON DELETE NO ACTION ON UPDATE NO ACTION;
