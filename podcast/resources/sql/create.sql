
CREATE TABLE IF NOT EXISTS localchannel (
    id INTEGER unique,
    title TEXT,
    feedurl TEXT,
    description TEXT,
    episodeCount INTEGER,

    iconUrl TEXT,
    websiteUrl TEXT,
    date_created DATETIME,
    valid BOOLEAN,
    pullOK INTEGER,
    pullFail INTEGER
);


-- all the fields contained in the xml file.
CREATE TABLE IF NOT EXISTS episodes (
    id INTEGER unique,
    title TEXT,
    mediafileUrl TEXT,
    channelid INTEGER REFERENCES localchannel(id),
    description TEXT,

    mediaSize  INTEGER,
    mediaDuration INTEGER,
    play_position INTEGER,
    filesize INTEGER,
    duration INTEGER,

    iconUrl TEXT,
    websiteUrl TEXT,
    webpageUrl TEXT,
    date_created DATETIME,
    date_published DATETIME,
    valid BOOLEAN,

    cached BOOLEAN,
    cacheLocation TEXT,

    pullOK INTEGER,
    pullFail INTEGER
);

CREATE TABLE IF NOT EXISTS episodes_notes (
    id INTEGER unique,
    episodeid INTEGER REFERENCES episodes(id),
    edit_time DATETIME,
    note TEXT
);

-- change table: add column.
-- ALTER TABLE localchannel ADD update_since_boot INTEGER NULL;

-- CREATE TABLE IF NOT EXISTS channel-config ();
-- updatetime.

-- CREATE TABLE IF NOT EXISTS episodes-config ();
-- notes: favourite. stars. lock for no delete, etc.

-- CREATE TABLE IF NOT EXISTS playhistory ();


-- CREATE TABLE IF NOT EXISTS notes ();


