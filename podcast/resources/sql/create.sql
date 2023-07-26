
CREATE TABLE IF NOT EXISTS localchannel (
    id INTEGER unique,
    title TEXT,
    feedurl TEXT,
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
    title TEXT,
    mediafileUrl TEXT,
    channelid INTEGER REFERENCES localchannel(id),
    description TEXT,

    mediaSize  INTEGER,
    mediaDuration INTEGER,
    cached BOOLEAN,
    cacheLocation TEXT,
    filesize INTEGER,
    duration INTEGER,

    iconUrl TEXT,
    websiteUrl TEXT,
    webpageUrl TEXT,
    date_created DATETIME,
    date_published DATETIME,
    valid BOOLEAN,

    pullOK INTEGER,
    pullFail INTEGER

) ;

