CREATE TABLE IF NOT EXISTS notes (
  id STRING UNIQUE,
  title TEXT,
  text TEXT,
  date_created DATETIME,
  date_modified DATETIME,
  notebook STRING REFERENCES notebooks(id),
  favorited BOOLEAN,
  encrypted BOOLEAN,
  trashed BOOLEAN
);

CREATE TABLE if NOT EXISTS notebooks (
  id INTEGER unique,
  title TEXT,
  date_modified DATETIME,
  parent STRING REFERENCES notebooks(id),
  row INTEGER,
  encrypted BOOLEAN
);

CREATE TABLE IF NOT EXISTS tags (
  id INTEGER UNIQUE,
  title TEXT,
  date_modified DATETIME,
  row INTEGER,
  encrypted BOOLEAN
);

-- Many-to-many relationship between notes and tags
CREATE TABLE IF NOT EXISTS notes_tags (
  note STRING REFERENCES notes(id),
  tag INTEGER REFERENCES tags(id)
);

CREATE TABLE IF NOT EXISTS localchannel ();
CREATE TABLE IF NOT EXISTS episode ();

