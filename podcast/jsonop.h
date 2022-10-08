#pragma once

#include <QString>

class PodData;
class EpisodeData;

bool jsonload(PodData *entity, const QString &filepath);
bool jsonsave(PodData *entity, const QString &filepath);


bool saveBookmarks();
bool loadBookmarks();

bool saveNotes();
bool loadNotes();
//download:
// xml => entity => json

// startup:
// json => entity.
// close:
// entity =>json.
