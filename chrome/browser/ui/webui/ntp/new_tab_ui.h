// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_UI_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_UI_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_ui_controller.h"

class GURL;
class Profile;

namespace base {
class DictionaryValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

// The WebUIController used for the New Tab page.
class NewTabUI : public content::WebUIController,
                 public content::WebContentsObserver,
                 public content::NotificationObserver {
 public:
  explicit NewTabUI(content::WebUI* web_ui);
  virtual ~NewTabUI();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // Returns whether or not this browser process might ever need to show apps
  // on the NTP.  With Win8 running in immersive mode, for example, apps are
  // displayed on a ChromeOS-style apps bar and not on the NTP.  In desktop,
  // however, apps are displayed on the NTP.  Since they both share the same
  // browser process instance, a different decision is necessary for whether
  // or not to register app message handlers versus whether to show it on NTP.
  static bool MightShowApps();

  // Returns whether or not to show apps pages.
  static bool ShouldShowApps();

  // Returns whether or not "Discovery" in the NTP is Enabled.
  static bool IsDiscoveryInNTPEnabled();

  // Adds "url", "title", and "direction" keys on incoming dictionary, setting
  // title as the url as a fallback on empty title.
  static void SetUrlTitleAndDirection(base::DictionaryValue* dictionary,
                                      const base::string16& title,
                                      const GURL& gurl);

  // Adds "full_name" and "full_name_direction" keys on incoming dictionary.
  static void SetFullNameAndDirection(const base::string16& full_name,
                                      base::DictionaryValue* dictionary);

  // Returns a pointer to a NewTabUI if the WebUIController object is a new tab
  // page.
  static NewTabUI* FromWebUIController(content::WebUIController* ui);

  // The current preference version.
  static int current_pref_version() { return current_pref_version_; }

  // WebUIController implementation:
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReused(
      content::RenderViewHost* render_view_host) OVERRIDE;

  // WebContentsObserver implementation:
  virtual void WasHidden() OVERRIDE;

  bool showing_sync_bubble() { return showing_sync_bubble_; }
  void set_showing_sync_bubble(bool showing) { showing_sync_bubble_ = showing; }

  class NewTabHTMLSource : public content::URLDataSource {
   public:
    explicit NewTabHTMLSource(Profile* profile);
    virtual ~NewTabHTMLSource();

    // content::URLDataSource implementation.
    virtual std::string GetSource() const OVERRIDE;
    virtual void StartDataRequest(
        const std::string& path,
        int render_process_id,
        int render_frame_id,
        const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
    virtual std::string GetMimeType(const std::string&) const OVERRIDE;
    virtual bool ShouldReplaceExistingSource() const OVERRIDE;
    virtual bool ShouldAddContentSecurityPolicy() const OVERRIDE;

    // Adds |resource| to the source. |resource_id| is resource id or 0,
    // which means return empty data set. |mime_type| is mime type of the
    // resource.
    void AddResource(const char* resource,
                     const char* mime_type,
                     int resource_id);

   private:
    // Pointer back to the original profile.
    Profile* profile_;

    // Maps resource files to mime types an resource ids.
    std::map<std::string, std::pair<std::string, int> > resource_map_;

    DISALLOW_COPY_AND_ASSIGN(NewTabHTMLSource);
  };

 private:
  FRIEND_TEST_ALL_PREFIXES(NewTabUITest, UpdateUserPrefsVersion);

  // content::NotificationObserver implementation.
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  // If |web_contents| has an NTP URL, emits a number of NTP statistics (like
  // mouseovers counts) associated with |web_contents|, to be logged in UMA
  // histograms.
  void EmitNtpStatistics();

  void OnShowBookmarkBarChanged();

  void StartTimingPaint(content::RenderViewHost* render_view_host);
  void PaintTimeout();

  Profile* GetProfile() const;

  content::NotificationRegistrar registrar_;

  // The time when we started benchmarking.
  base::TimeTicks start_;
  // The last time we got a paint notification.
  base::TimeTicks last_paint_;
  // Scoping so we can be sure our timeouts don't outlive us.
  base::OneShotTimer<NewTabUI> timer_;
  // The preference version. This used for migrating prefs of the NTP.
  static const int current_pref_version_ = 3;

  // If the sync promo NTP bubble is being shown.
  bool showing_sync_bubble_;

  PrefChangeRegistrar pref_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(NewTabUI);
};

#endif  // CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_UI_H_
