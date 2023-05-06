#pragma once

#include <QString>

class PodcastChannel;
class EpisodeData;

bool jsonload(PodcastChannel *entity, const QString &filepath);
bool jsonsave(PodcastChannel *entity, const QString &filepath);

bool saveBookmarks();
bool loadBookmarks();

bool saveNotes();
bool loadNotes();
// download:
//  xml => entity => json

// startup:
// json => entity.
// close:
// entity =>json.
