#include "config.h"
#ifdef ENABLE_AMAROK1
#include <stdio.h>
#include <glib.h>
#include "ol_player_amarok1.h"
#include "ol_utils_dcop.h"
#include "ol_debug.h"

static const char TITLE_CMD[] = "title";
static const char ARTIST_CMD[] = "artist";
static const char ALBUM_CMD[] = "album";
static const char TRACK_CMD[] = "track";
static const char PLAYED_TIME_CMD[] = "trackCurrentTimeMs";
static const char MUSIC_LENGTH_CMD[] = "trackTotalTime";
static const char IS_PLAYING_CMD[] = "isPlaying";

static gboolean ol_player_amarok1_get_capacity ();
static gboolean ol_player_amarok1_get_music_info (OlMusicInfo *info);
static gboolean ol_player_amarok1_get_played_time (int *played_time);
static gboolean ol_player_amarok1_get_music_length (int *music_length);
static gboolean ol_player_amarok1_get_activated ();
/** 
 * @brief Generates the full DCOP command for AmarOK 1.4 from short player commands
 * If the cmd is `foo', then the generated command will be `dcop amarok player foo'
 * @param cmd The short player command
 * 
 * @return Generated command, must be freed by g_free
 */
static char* ol_player_amarok1_get_command (const char *cmd);
/** 
 * @brief Gets the returned string by amarok command
 * 
 * @param cmd The short player command
 * @param returnval The pointer to the string, must not be NULL.
 *                  The original string will be destroyed, and a new string will be allocated.
 *                  To destroy the returned string, use g_free
 * 
 * @return TRUE if success
 */
static gboolean ol_player_amarok1_get_string (const char *cmd, char **returnval);
static gboolean ol_player_amarok1_get_uint (const char *cmd, guint *returnval);
static gboolean ol_player_amarok1_get_boolean (const char *cmd, gboolean *returnval);

static char*
ol_player_amarok1_get_command (const char *cmd)
{
  ol_assert_ret (cmd != NULL, NULL);
  return g_strdup_printf ("dcop amarok player %s", cmd);
}

static gboolean
ol_player_amarok1_get_string (const char *cmd, char **returnval)
{
  ol_assert_ret (cmd != NULL, FALSE);
  ol_assert_ret (returnval != NULL, FALSE);
  char *dcop_cmd = ol_player_amarok1_get_command (cmd);
  if (*returnval != NULL)
  {
    g_free (*returnval);
    *returnval = NULL;
  }
  gboolean ret = ol_dcop_get_string (dcop_cmd, returnval);
  g_free (dcop_cmd);
  return ret;
}

static gboolean
ol_player_amarok1_get_uint (const char *cmd, guint *returnval)
{
  ol_assert_ret (cmd != NULL, FALSE);
  ol_assert_ret (returnval != NULL, FALSE);
  char *dcop_cmd = ol_player_amarok1_get_command (cmd);
  gboolean ret = ol_dcop_get_uint (dcop_cmd, returnval);
  g_free (dcop_cmd);
  return ret;
}

static gboolean
ol_player_amarok1_get_boolean (const char *cmd, gboolean *returnval)
{
  ol_assert_ret (cmd != NULL, FALSE);
  ol_assert_ret (returnval != NULL, FALSE);
  char *dcop_cmd = ol_player_amarok1_get_command (cmd);
  gboolean ret = ol_dcop_get_boolean (dcop_cmd, returnval);
  g_free (dcop_cmd);
  return ret;
}

static gboolean
ol_player_amarok1_get_music_info (OlMusicInfo *info)
{
  ol_assert_ret (info != NULL, FALSE);
  ol_music_info_clear (info);
  if (!ol_player_amarok1_get_string (TITLE_CMD, &info->title))
    return FALSE;
  if (!ol_player_amarok1_get_string (ARTIST_CMD, &info->artist))
    return FALSE;
  if (!ol_player_amarok1_get_string (ALBUM_CMD, &info->album))
    return FALSE;
  if (!ol_player_amarok1_get_uint (TRACK_CMD, &info->track_number))
    return FALSE;
  ol_debugf ("  title: %s\n"
             "  artist: %s\n"
             "  album: %s\n",
             ol_music_info_get_title (info),
             ol_music_info_get_artist (info),
             ol_music_info_get_album (info));
  return TRUE;
}

static gboolean
ol_player_amarok1_get_played_time (int *played_time)
{
  ol_assert_ret (played_time != NULL, FALSE);
  if (!ol_player_amarok1_get_uint (PLAYED_TIME_CMD, played_time))
    return FALSE;
  return TRUE;
}

static gboolean
ol_player_amarok1_get_music_length (int *music_length)
{
  ol_assert_ret (music_length != NULL, FALSE);
  if (!ol_player_amarok1_get_uint (MUSIC_LENGTH_CMD, music_length))
    return FALSE;
  *music_length *= 1000;
  return TRUE;
}
static gboolean
ol_player_amarok1_get_activated ()
{
  gboolean is_playing = FALSE;;
  if (!ol_player_amarok1_get_boolean (IS_PLAYING_CMD, &is_playing) ||
      !is_playing)
    return FALSE;
  else
    return TRUE;
}

static int
ol_player_amarok1_get_capacity ()
{
  return 0;
}

struct OlPlayer*
ol_player_amarok1_get ()
{
  ol_log_func ();
  struct OlPlayer *controller = ol_player_new ("Amarok 1.4");
  ol_player_set_cmd (controller, "amarok");
  controller->get_music_info = ol_player_amarok1_get_music_info;
  controller->get_activated = ol_player_amarok1_get_activated;
  controller->get_played_time = ol_player_amarok1_get_played_time;
  controller->get_music_length = ol_player_amarok1_get_music_length;
  controller->get_capacity = ol_player_amarok1_get_capacity;
  controller->get_status = NULL;
  controller->play = NULL;
  controller->pause = NULL;
  controller->next = NULL;
  controller->prev = NULL;
  controller->seek = NULL;
  return controller;
}
#endif  /* ENABLE_AMAROK1 */
