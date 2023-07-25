
CREATE TABLE IF NOT EXISTS localchannel (
    id INTEGER unique,
    feedurl TEXT,
    title TEXT,
    description TEXT,
    iconUrl TEXT,
    websiteUrl TEXT,
    date_created DATETIME,
    valid BOOLEAN,
    pullOK INTEGER,
    pullFail INTEGER
);

CREATE TABLE IF NOT EXISTS episodes (
    id INTEGER unique,
    feedurl TEXT,
    title TEXT,
    description TEXT,
    iconUrl TEXT,
    websiteUrl TEXT,
    date_created DATETIME,
    date_published DATETIME,
    valid BOOLEAN,
    pullOK INTEGER,
    pullFail INTEGER

) ;

