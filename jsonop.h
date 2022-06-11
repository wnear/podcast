#pragma once

#include <QString>

class PodData;
class EpisodeData;

bool jsonload(PodData *entity, const QString &filepath);
bool jsonsave(PodData *entity, const QString &filepath);


bool loadBookmarks();
bool saveBookmarks();

bool loadNotes();
bool saveNotes();
//download:
// xml => entity => json

// startup:
// json => entity.
// close:
// entity =>json.
