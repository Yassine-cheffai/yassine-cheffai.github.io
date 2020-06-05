/**
 ----------------------------------------------------------------
 Team Render Server Web Frontend
 ----------------------------------------------------------------
 */

application = (/** @lends application */
	function() {
		/**
		 The application - to be instanciated after document load.
		 Handles global state, events and server-communication.

		 Content views (pages) are held in 'sections', the application
		 singleton can be accessed from anywhere via the 'window.app' handle.

		 @constructor
		 */
		function application() {
			// version string
			this.version = "2.01";
			// config JSO
			this.config = {
				// debugging
				debug : false,
				// global pulling timer
				timer : 10000,
				// global pulling timeout
				timeout : 999999,
				// cookie prefix (you are never alone on localhost)
				cookiePrefix : "TRWI_",
				// session cookie timeout (in days)
				sessionTimout : 0.04,
				// default section
				defaultSection : "monitor",
				// language
				language : navigator.language || navigator.userLanguage,
				// is login required?
				loginRequired : $("#users").exists()
			},
			// error state
			this.error = false;
			// language dict
			this.lang = null;
			// language fallback dict
			this.lang_default = null;
			// auth
			this.user = null;
			// section marker
			this.actualSection = {
				name : "none"
			};
			// section cache
			this.oldSection = {
				name : "none"
			};
			// app modules
			this.notifications = null;
			this.navigation = null;
			// sections
			this.sections = {};
			this.sections.info = null;
			this.sections.jobs = null;
			this.sections.monitor = null;
			this.sections.network = null;
			if (this.config.loginRequired) {
				this.sections.login = null;
				this.sections.users = null;
				this.sections.logout = null;
			}
			// timer
			this.timer = null;
			// running uploads
			this.runningUploads = 0;
			// set handy global reference (singleton)
			if (window.app) {
				utils.log("'application' can only be initialized once!");
			} else {
				// app will be used exclusively from now on to simplify things
				window.app = this;
			}
			// auto init
			app.init();
		}

		/**
		 Global init - should be called after document load.
		 */
		application.prototype.init = function() {
			// show version
			$("#footer").find("#version").text("TR-WI " + app.version);
			// init notifications
			app.notifications = new application.notifications();
			// get localized strings and try to restore the session
			app.getLanguage(app.restoreSession);
		};

		/**
		 Displays the (default) section and initializes the navigation bar.
		 */
		application.prototype.launch = function() {
			// init sections
			app.sections.info = new application.section.info();
			app.sections.jobs = new application.section.jobs();
			app.sections.network = new application.section.network();
			if (app.config.loginRequired) {
				app.sections.users = new application.section.users();
				app.sections.logout = new application.section.logout();
			}
			app.sections.monitor = new application.section.monitor();

			// init the navbar
			app.navigation = new application.navigation();
			// got session?
			if (app.user) {
				app.checkVisibility();
				app.gotoSection(utils.getCurrentHashWithoutParams());
			} else {
				if (app.sections.users) {
					app.gotoSection(app.sections.login.name);
				} else {
					app.gotoSection(app.sections.monitor.name);
				}
			}
			// debugging
			app.notifications.handleAllExceptions();
			
			// register global handlers
			app.registerHandlers();
		};

		/**
		 Loads a language JSO from the server.
		 @param {Function} callback
		 */
		application.prototype.getLanguage = function(callback) {
			utils.request("/languages", app, function(data) {
				var language = app.config.language.toLowerCase();

				app.languages = data.languages;
				if (app.user && app.user.language) {
					language = app.user.language;
				} else if (utils.getCookie("language")) {
					language = utils.getCookie("language");
				} else {
					for (var i = 0, length = app.languages.length; i < length; i++) {
						var current_lang = app.languages[i];
						if (current_lang.default_language) {
							language = current_lang.extensions;
						}
					}
				}
				utils.request("/language?id=" + language, app, function(data) {
					app.lang = data.labels;
					if (language === "us") {
						// save a temp copy of the default language (English) as fallback
						app.lang_default = app.lang;
					}

					if (app.lang_default == null) {
						utils.request("/language?id=us", app, function(data) {
							app.lang_default = data.labels;
							// call cb if set
							if ( typeof callback == "function") {
								callback.call(app);
							};
						}, undefined,
						{
							cache: true
						});
					} else {
						// call cb if set
						if ( typeof callback == "function") {
							callback.call(app);
						};
					}
				}, undefined,
				{
					cache: true
				});
			}, undefined,
			{
				cache: true
			});
		};

		/**
		 Restores a previous session from cookies (on F5 reload).
		 */
		application.prototype.restoreSession = function() {
			// translate templates
			utils.translateElements($("body"));
			// now that we are localized, init login section
			if (app.config.loginRequired) {
				app.sections.login = new application.section.login();
				// try to restore user from cookies
				var oldusername = utils.getCookie("username");
				var oldisadmin = utils.getCookie("isadmin");
				var olduuid = utils.getCookie("uuid");
				var oldlanguage = utils.getCookie("language");
				// found old session
				if (oldusername) {
					// verify
					utils.request("/verify", this, function() {
						// verified old session - restore user data
						if (!app.user) {
							app.user = {};
						}
						if (oldusername) {
							app.user.username = oldusername;
						}
						if (oldisadmin) {
							app.user.isadmin = (oldisadmin === "true");
						}
						if (olduuid) {
							app.user.uuid = olduuid;
						}
						if (oldlanguage) {
							app.user.language = oldlanguage;
						}
						// show user label
						app.sections.login.showUserName();
						// launch (old session restored)
						app.launch();
					}, function() {
						// server declined - launch (new session)
						app.launch();
					});
				} else {
					// launch (new session)
					app.launch();
				}
			} else {
				app.user = {}
				app.user.username = "admin";
				app.user.isadmin = true;
				app.launch();
			}
			
		};

		/**
		 Stores the actual session via cookies.
		 */
		application.prototype.storeSession = function() {
			// store session cookies
			if (app && app.user) {
				utils.setCookie("username", app.user.username, app.config.sessionTimout);
				utils.setCookie("isadmin", app.user.isadmin, app.config.sessionTimout);
				utils.setCookie("uuid", app.user.uuid, app.config.sessionTimout);
				utils.setCookie("language", app.user.language, app.config.sessionTimout);
			}
		};

		/**
		 Removes the actual session (clean logout).
		 @param {Boolean} reload - set if the page should be reloaded
		 */
		application.prototype.killSession = function(reload) {
			// kill auth
			app.user = null;
			// remove session cookies
			utils.removeCookie("username");
			utils.removeCookie("isadmin");
			utils.removeCookie("uuid");
			// reload 'n reset state
			if (reload) {
				window.location.reload();
			}
		};

		/**
		 Switches the view.
		 @param {String} sectionName - name of the section
		 @param {Boolean} setHash - sets the location hash
		 */
		application.prototype.gotoSection = function(sectionName, setHash) {
			// already there?
			if (app.actualSection.name === sectionName) {
				return;
			}
			var found = false;
			var selected = null;
			// check auth
			if (app.user) {
				// hide all sections and check target exists
				selected = app.sections[app.config.defaultSection];
				$.each(app.sections, function(key, value) {
					if (!app.sections.logout || sectionName !== app.sections.logout.name) {
						value.hide();
					}
					// found
					if (value.name === sectionName) {
						selected = value;
						found = true;
					} else {
						// section switch! clear all notifications from other sections
						app.notifications.clear(value.name);
					}
				});
			} else {
				// logged out by server - hide ALL sections
				$("#content").find(".static").hide();
				if (app.sections.users) {
					selected = app.sections.login;
				} else {
					selected = app.sections.jobs;
				}
				found = true;
			}
			if (!app.sections.logout || sectionName !== app.sections.logout.name) {
				// set cache section
				app.oldSection = app.actualSection.name;
				// set selected section
				app.actualSection = selected;
				// initialize the section with values
				app.actualSection.initValues();
				// set and fade in actual section
				app.actualSection.set();
				// inform navbar to update highlight (if already initialized)
				if (app.navigation) {
					app.navigation.setSection(app.actualSection.name);
				}
				// fix for navbar jump on mobile
				$("html, body").animate({
					scrollTop : 0
				});
				// set location hash?
				if (setHash || !found) {
					location.hash = "#/" + app.actualSection.name;
				}
			} else {
				if (app.sections.logout) {
					app.sections.logout.set();
				}
			}
		};

		/**
		 Forces the active section to request actual data.
		 */
		application.prototype.pull = function(timeRunloop) {
			// pull data by actual section (if section supports this)
			if (app.user && app.actualSection && typeof app.actualSection.pull === "function") {
				app.actualSection.pull(timeRunloop);
			}
		};

		/**
		 Triggers 'pull' after set timeout - used to prevent request stacking.
		 @param {Number} timeout
		 */
		application.prototype.pump = function(timeout) {
			timeout = timeout || app.config.timer;
			// set new timeout
			clearTimeout(this.timer);
			this.timer = setTimeout(function() {
    			app.pull(true);
			}, timeout);
		};

		/**
		 Shows/hides the loading spinner.
		 @param {Boolean} spinning - true for visible
		 */
		application.prototype.spin = function(spinning) {
			var $spinner = $("#navigation").find("#navbar-spinner img");
			$spinner.removeClass("hidden");
			spinning ? $spinner.show().fadeIn(250) : $spinner.fadeOut(250);
		};

		/**
		 Registers global handlers.
		 */
		application.prototype.registerHandlers = function() {
			// handle page resizing
			app.handleResize();
			// global click event handler to refresh session cookies
			$(document).click(function() {
				app.storeSession();
			});
			// global search input field handler (for clear btn)
			$(document).on("input search", ".filter", function() {
				$(this).next().toggleClass("hidden", !(this.value && $(this).val().length !== 0));
			}).on("click", ".btn_clear", function(e) {
				$(this).addClass("hidden");
				$(this).prev().val("").trigger("search");
			});
			// disable file drag'n'drop for the page
			$(document).bind("drop dragover", function(e) {
				e.preventDefault();
			});
			// before unload
			$(window).bind("beforeunload", function() {
				// save info somewhere
				if (app.runningUploads > 0) {
					return (app.runningUploads > 1 ? t("IDS_WEB_JOB_STILL_UPLOADING_MULTI", app.runningUploads) : t("IDS_WEB_JOB_STILL_UPLOADING"));
				}
			});
			
		};

		/**
		 Registers for $(window).resize event.
		 */
		application.prototype.handleResize = function() {
			// register event handler
			var timer;
			$(window).resize(function() {
				clearTimeout(timer);
				timer = setTimeout(app.resize, 15);
			})
			// first draw
			app.resize();
		};

		/**
		 Dynamically resizes the main content container.
		 */
		application.prototype.resize = function() {
			// notify all sections

			$.each(app.sections, function(key, value) {
				value.resize();
			});
			// position container
			var height = $("#navigation .navbar").height();
			$("#content").offset({top:height});
			
			
		};

		/**
		 Removes all elements not ment to be visible at current user level from the DOM.
		 */
		application.prototype.checkVisibility = function() {
			// notify all sections
			$.each(app.sections, function(key, value) {
				if ( typeof value.checkVisibility === "function") {
					value.checkVisibility();
				}
			});
		};

		return application;
	})();

application.notifications = (/** @lends application.notifications */
	function() {
		/**
		 Notification bar (seperate from content / sections).

		 Spawns notifications in a floating div under the navigation bar.
		 Notifications can be timeout- / user-dismissed or permanent.

		 Note: all 'message' calls return the notification dom element as $
		 for reference - this is handy to remove permanent notifications later.
		 @constructor
		 */
		function notifications() {
			// element container
			this.$container = null;
			// template
			this.$template = null;
			// singleton
			this.initialized = false;
			// init
			this.init();
		}

		/**
		 Init - should be called after document load.
		 */
		notifications.prototype.init = function() {
			// singleton
			if (this.initialized) {
				utils.log("'notifications' can only be initialized once!");
				return;
			} else {
				this.initialized = true;
			}
			// container
			this.$container = $("#notifications").find(".static");
			this.$container.show();
			// template
			this.$template = $("#notifications").find("#notification_template");
		};

		/**
		 Displays a 'success' message.
		 @param {String} str - the message
		 @param {Number} timeout - the notification timeout - <0 for inf.
		 @param {String} id - an optional identifier for clear()
		 @param {Boolean} nodismiss - not dismissable
		 @returns {Object} the message element $
		 */
		notifications.prototype.success = function(str, timeout, id, nodismiss) {
			return this.message(str, "success", timeout, id, nodismiss);
		};

		/**
		 displays a 'info' message.
		 @param {String} str - the message
		 @param {Number} timeout - the notification timeout - <0 for inf.
		 @param {String} id - an optional identifier for clear()
		 @param {Boolean} nodismiss - not dismissable
		 @returns {Object} the message element $
		 */
		notifications.prototype.info = function(str, timeout, id, nodismiss) {
			return this.message(str, "info", timeout, id, nodismiss);
		};

		/**
		 Displays a 'warning' message.
		 @param {String} str - the message
		 @param {Number} timeout - the notification timeout - <0 for inf.
		 @param {String} id - an optional identifier for clear()
		 @param {Boolean} nodismiss - not dismissable
		 @returns {Object} the message element $
		 */
		notifications.prototype.warning = function(str, timeout, id, nodismiss) {
			return this.message(str, "warning", timeout, id, nodismiss);
		};

		/**
		 Displays a 'danger' message.
		 @param {String} str - the message
		 @param {Number} timeout - the notification timeout - <0 for inf.
		 @param {String} id - an optional identifier for clear()
		 @param {Boolean} nodismiss - not dismissable
		 @returns {Object} the message element $
		 */
		notifications.prototype.danger = function(str, timeout, id, nodismiss) {
			return this.message(str, "danger", timeout, id, nodismiss);
		};

		/**
		 Displays a message.
		 @param {String} str - the message
		 @param {String} level - ("success", "info", "warning" or "danger")
		 @param {String} id - an optional identifier for clear()
		 @param {Number} timeout - the notification timeout - <0 for inf.
		 @param {Boolean} nodismiss - not dismissable
		 @returns {Object} the message element $
		 */
		notifications.prototype.message = function(str, level, timeout, id, nodismiss) {
			level = level || "info";
			timeout = timeout || 5000;
			// switch alert classes
			var $templ = $(this.$template.html());
			$templ.removeAttr("class");
			$templ.addClass("alert alert-" + level + " alert-dismissable");
			if (nodismiss) {
				$templ.removeClass("alert-dismissable");
				$templ.find("button").remove();
			}
			// set message
			$templ.find(".notification-span_message").text(str);
			// set id
			if (id) {
				$templ.attr("id", id);
			}
			// to dom
			var $obj = this.$container.append($templ).children().last();
			if (!nodismiss) {
				$obj.click(function() {
					$(this).fadeOut().remove();
				});
			}
			// no sel
			$obj.disableSelection();
			$obj.fadeIn(500);
			// fade out after timeout
			if (timeout > 0) {
				setTimeout(function() {
					$obj.fadeOut(500, function() {
						$obj.remove();
					});
				}, timeout);
			}
			return $obj;
		};

		/**
		 Clears all notifications.
		 @param {String} id - an optional identifier - clears only the id
		 */
		notifications.prototype.clear = function(id) {
			if (id) {
				this.$container.find("#" + id).remove();
			} else {
				this.$container.html("");
			}
		};

		/**
		 Pipe all exceptions to a notification.
		 @param {Boolean} spinning - true for visible
		 */
		notifications.prototype.handleAllExceptions = function() {
			var scope = this;
			window.onerror = function(message, url, lineNumber) {
				if (app.config.debug) {
					scope.danger(message, -1);
				} else {
					if (!app.error) {
						scope.danger(t("IDS_WEB_EXCEPTION"), -1);
					}
				}
				app.error = true;
			};
		};

		return notifications;
	})();

application.navigation = (/** @lends application.navigation */
	function() {
		/**
		 Navigation bar (seperate from content / sections).
		 Auto-populates the navigation bar with section links and handles highlighting.

		 Note: must be instanciated after the sections!
		 @constructor
		 */
		function navigation() {
			// buttons JSO
			this.btns = {};
			// elements
			this.$toggle = null;
			// singleton
			this.initialized = false;
			// init manually after login
			this.init();
		}

		/**
		 Init - should be called after sections have been initialized.
		 */
		navigation.prototype.init = function() {
			// singleton
			if (this.initialized) {
				utils.log("'navigation' can only be initialized once!");
				return;
			} else {
				this.initialized = true;
			}
			// elements
			this.$toggle = $("#navigation").find("#navbar-toggle");
			// btns
			var scope = this;
			
			$("#navigation li a").each(function() {
				var $this = $(this);
				var section = app.sections[$this.data("target")];
				$this.attr("href", "#/" + section.name);
				$this.click(function(e) {
					e.preventDefault();
					app.gotoSection(section.name, !app.sections.logout || section.name !== app.sections.logout.name);
				});
				// store for highlighting
				scope.btns[section.name] = $this;
			});
			// check vis
			this.checkVisibility();
		};

		/**
		 Hide elements not visible to current user level.
		 */
		navigation.prototype.checkVisibility = function() {
			var scope = this;
			$.each(this.btns, function(key, value) {
				$el = $(value);
				// hack for user-profile
				if (app.user && !app.user.isadmin && $el.text() === utils.capitalizeFirstChar(app.sections.users.name)) {
					$el.find("a").text("Profile");
				}
				// hide for login
				if (app.user) {
					$el.show();
					scope.$toggle.show();
				} else {
					$el.hide();
					scope.$toggle.hide();
				}
			});
		};

		/**
		 Highlights the active section.
		 @param {String} sectionName - the section name
		 */
		navigation.prototype.setSection = function(sectionName) {
			// set all btns inactive
			$("#navigation").find(".navbar-nav li").removeClass("active");
			// find and highlight active section btn
			$.each(this.btns, function(key, value) {
				if (key === sectionName) {
					value.closest("li").addClass("active");
					return false;
				}
			});
			// check vis
			this.checkVisibility();
		};

		return navigation;
	})();

application.section = (/** @lends application.section */
	function() {
		/**
		 Section object prototype.

		 Base section object - all sections inherit from this.
		 @constructor
		 */
		function section() {
			// section name
			this.verbose_name = "IDS_WEB_INFO";
			this.name = "info";
			// section data url
			this.url = "";
			// section link in navbar?
			this.linkInNavBar = true;
			// element container
			this.$container = null;
			// templates JSO
			this.templates = {};
			// singleton
			this.initialized = false;
		};

		/**
		 InitValues - called whenever a section is loaded.
		 */
		section.prototype.initValues = function() {

		}

		/**
		 Init - should be called after document load.
		 */
		section.prototype.init = function() {
			// singleton
			if (this.initialized) {
				utils.log("section '" + this.name + "' can only be initialized once!");
				return;
			} else {
				this.initialized = true;
			}
			// get container
			this.$container = $("#content").find("#" + this.name).find(" .static");
			// translate
			this.translate();
		};

		/**
		 Shows the content.
		 */
		section.prototype.show = function() {
			this.$container.fadeIn();
		};

		/**
		 Hides the content.
		 */
		section.prototype.hide = function() {
			this.$container.hide();
		};

		/**
		 Makes the section fit the browser viewport.
		 */
		section.prototype.resize = function() {
			// overwrite this
		};

		/**
		 Sets the section active (old sections have to be hidden seperately).
		 @returns {Object}
		 */
		section.prototype.set = function() {
			// show
			this.show();
		};

		section.prototype.translate = function() {
			// translate static
			utils.translateElements(this.$container);
			// translate templates
			$templates = this.$container.parent().find(".templates");
			if ($templates.exists()) {
				utils.translateElements($templates);
			}
		};

		return section;
	})();

application.ajaxSection = (/** @lends application.ajaxSection */
	function() {
		ajaxSection.prototype = Object.create(application.section.prototype);
		/**
		 Ajax autp pulling section object prototype.

		 Base ajax section object - all ajax sections inherit from this.
		 @constructor
		 */
		function ajaxSection() {
			// call super
			application.section.call(this);
			// raw data
			this.rawData = null;
			// raw data cache
			this.rawDataCache = null;
			// JSON data
			this.data = null;
			// Ajax request handler
			this.requestHandler = null;
		};

		/**
		 Sets the section active (old sections have to be hidden seperately).
		 @returns {Object}
		 */
		ajaxSection.prototype.set = function() {
			// call super
			application.section.prototype.set.call(this);
			// update data
			this.pull();
		};

		/**
		 Pull new data.
		 */
		ajaxSection.prototype.pull = function() {
			this.cancelPull();
			app.spin(true);
			this.requestHandler = utils.request(this.url, this, [this.setData, this.lazyUpdate], this.pullError);
		};

		/**
		 Handle data error.
		 @param {Object} e
		 @param {Object} status
		 @param {Object} error
		 @param {String} url
		 */
		ajaxSection.prototype.pullError = function(e, status, error, url) {
			utils.throwHTTPError(e, status, error, url);
			app.spin(false);
			app.pump();
		};

		/**
		 Sets the sections JSON data buffer.
		 @param {Object} data - JSON JSO
		 */
		ajaxSection.prototype.setData = function(data) {
			// spin
			app.spin(false);
			// set data
			this.rawDataCache = this.rawData;
			this.rawData = JSON.stringify(data);
			this.data = data;
		};

		/**
		 Updates the content.
		 */
		ajaxSection.prototype.lazyUpdate = function() {
			// data newer than cache?
			if (this.rawData !== this.rawDataCache) {
				// draw
				this.update();
			}
			app.pump();
		};

		/**
		 Clears the raw JSON cache - forces a redraw on next pull().
		 */
		ajaxSection.prototype.invalidateCache = function() {
			this.rawDataCache = null;
		};

		/**
		 Blocks updates.
		 @param {Boolean} invalidateCache - clears the JSON cache and forces a redraw on next pull()
		 @param {Boolean} clearIntervall - also clears the running pump timer
		 */
		ajaxSection.prototype.cancelPull = function(invalidateCache, clearIntervall) {
			// abort existing requests
			if (this.requestHandler) {
				this.requestHandler.abort();
			}
			// clear cache
			if (invalidateCache) {
				this.invalidateCache();
			}
			// clear intervall
			if (clearIntervall) {
				clearInterval(app.timer);
			}
		};

		return ajaxSection;
	})();

application.listSection = (/** @lends application.section */
	function() {
		listSection.prototype = Object.create(application.ajaxSection.prototype);
		/**
		 List section object prototype.

		 Base list section object - all list sections inherit from this.
		 @extends application.ajaxSection
		 @constructor
		 */
		function listSection() {
			// call super
			application.ajaxSection.call(this);
			// selected list item
			this.selectedItem = null;
			// left container (list)
			this.$listPane = null;
			// right container (details)
			this.$itemDetails = null;
			// selection
			this.$selectionControl = null;
			this.$selectionBtn = null;
			this.$selectionAll = null;
			this.$selectionNone = null;
			// filtering
			this.$filterField = null;
			// selected items
			this.selected = [];
			// already looked for item by location hash uuid?
			this.hashSearched = false;
			// selection and filter controls switch
			this.controlsVisible = false;
		};

		/**
		 Init - should be called after document load.
		 */
		listSection.prototype.init = function() {
			// call super
			application.ajaxSection.prototype.init.call(this);
			// get left container (list)
			this.$listPane = this.$container.find("#" + this.name + "-list");
			// get right container (details)
			this.$itemDetails = this.$container.find("#" + this.name + "-details").hide();
			// get item template
			this.templates.$item = this.$container.parent().find(".templates").find("#" + this.name + "-item_template");
			// elements
			this.$selectionControl = this.$container.find(".selection_control");
			this.$selectionBtn = this.$container.find(".select_btn");
			this.$selectionControlAll = this.$selectionControl.find(".select_all");
			this.$selectionControlNone = this.$selectionControl.find(".select_none");
			this.$filterField = this.$container.find(".scrollpane_control").find(".filter");
			// details are hidden until data is available
			
			this.$itemDetails.hide();
			// selection and filter also
			this.$selectionBtn.hide();
			this.$filterField.hide();
			// filtering
			var scope = this;
			this.$filterField.bind("keyup search", function() {
				scope.filter(scope.$filterField.val());
			});
			// selection controls
			this.$selectionControlAll.click(function(e) {
				e.preventDefault();
				scope.$listPane.find("input:visible").prop("checked", true);
				scope.setSelected();
			});
			this.$selectionControlNone.click(function(e) {
				e.preventDefault();
				scope.$listPane.find("input:visible").prop("checked", false);
				scope.setSelected();
			});
		};

		/**
		 Updates the sections content.
		 */
		listSection.prototype.update = function() {
			var focus = this.selectedItem === null;
			// select default item
			this.getSelectedItem(this.data[this.name]);
			// update list
			this.updateList();
			if (this.selectedItem) {
				// update item highlight
				this.selectItem(this.selectedItem);
				// update details
				this.$itemDetails.show();
				this.updateDetails(this.selectedItem);
			}
			// focus default selected item on first pull
			if (focus) {
				this.focusItem();
			}
			// resize
			this.resize();
			// update selection controls
			this.updateControls();
			this.handleCheckboxes();
			
			var scope = this;
		};

		/**
		 Update list.
		 */
		listSection.prototype.updateList = function() {
			// clear list
			this.$listPane.html("");
			// build list
			for (var i = 0; i < this.data[this.name].length; i++) {
				var item = this.data[this.name][i];
				// create item
				this.newItemFromTemplate(item, item.name, "#/" + this.name + "/");
			}
		};

		/**
		 Update detail page.
		 */
		listSection.prototype.updateDetails = function() {
			this.enableDetails();
		};

		/**
		 Sets the currently selected item.
		 @param {Object} item - the item to select
		 */
		listSection.prototype.selectItem = function(item) {
			// set selected
			this.selectedItem = item;
			// set details
			this.updateDetails(item);
			this.focusDetails();
			// get item $
			var $itemElement = this.getItemElementByUuid(item.uuid);
			if ($itemElement) {
				// hide all items
				this.$listPane.find(".panel").removeClass("panel-primary");
				// highlight selected item
				$itemElement.addClass("panel-primary");
			}
		};

		/**
		 Find an item by the given uuid.
		 @param {String} uuid
		 */
		listSection.prototype.getItemElementByUuid = function(uuid) {
			var itemElement = null;
			this.$listPane.find(".panel").each(function(index) {
				var itemUuid = $(this).find("a").prop("id");
				if (itemUuid === uuid) {
					itemElement = $(this);
					return false;
				}
			});
			return itemElement;
		};

		/**
		 Sets the default item - if possible from given location uuid.
		 Also updates the uuid hash if empty.
		 @param {Object} data - the item list
		 */
		listSection.prototype.getSelectedItem = function(data) {
			var selectedItem = data[0];
			if (!this.selectedItem) {
				// try hash only on first run
				if (!this.hashSearched) {
					this.hashSearched = true;
					for (var i = 0; i < data.length; i++) {
						if (location.href.indexOf(data[i].uuid) !== -1) {
							selectedItem = data[i];
							break;
						}
					}
					// update hash
					if (selectedItem) {
						location.hash = "#/" + this.name + "/" + selectedItem.uuid;
					}
				}
			} else {
				// check if the current selected item is still in the new list ...
				for (var i = 0; i < data.length; i++) {
					if (this.selectedItem.uuid === data[i].uuid) {
						selectedItem = data[i];
						break;
					}
				}
			}
			this.selectedItem = selectedItem;
		};

		/**
		 Store selection.
		 */
		listSection.prototype.setSelected = function() {
			// scroll to selection
			this.selected = [];
			var scope = this;
			this.$listPane.find(".panel").each(function(index) {
				if ($(this).find("input").prop("checked")) {
					var uuid = $(this).find("a").prop("href");
					uuid = uuid.substring(uuid.lastIndexOf("/") + 1);
					scope.selected.push(uuid);
				}
			});
			// update controls
			this.handleSelectionMenu();
		};

		/**
		 Restore selection.
		 */
		listSection.prototype.restoreSelected = function() {
			for (var i = 0; i < this.selected.length; i++) {
				// find uuid by anchor
				var itm = this.$listPane.find("a[href$='" + this.selected[i] + "']");
				if (itm.exists()) {
					itm.parents().eq(2).find("input").prop("checked", true);
				}
			}
			// update controls
			this.handleSelectionMenu();
		};
		/**
		 Clear selection.
		 */
		listSection.prototype.clearSelected = function() {
			this.$listPane.find(".panel input:checked").prop("checked", false);
			this.selected = [];
			this.handleSelectionMenu();
		};

		/**
		 Updates the section menu (enables/disables actions).

		 Note: Call after updateList()
		 */
		listSection.prototype.handleSelectionMenu = function() {
			if (this.controlsVisible) {
				var scope = this;
				if (this.selected.length === 0) {
					this.$selectionControl.find(".selected_action").addClass("disabled");
				} else {
					this.$selectionControl.find(".selected_action").removeClass("disabled");
					this.$selectionControl.find(".selected_action > a").each(function(index) {
						var type = $(this).attr("class");
						stype = type.substr(type.lastIndexOf("_") + 1, type.length);
						found = false;
						scope.$listPane.find("." + stype + "_able").each(function(index) {
							var uuid = $(this).find("a:first").attr("href");
							uuid = uuid.substring(uuid.lastIndexOf("/") + 1);
							if (scope.selected.indexOf(uuid) > -1) {
								found = true;
								return false;
							}
						});
						// disable
						if (!found) {
							scope.$container.find("." + type).parent().addClass("disabled");
						}
					});
				}
			}
		};

		/**
		 Adds checkbox events.

		 Note: call after updateList();
		 */
		listSection.prototype.handleCheckboxes = function() {
			if (this.controlsVisible) {
				this.restoreSelected();
				// add event to checkboxes
				var scope = this;
				this.$listPane.find("input").parent().click(function() {
					scope.setSelected();
				});
			} else {
				// selection controls are disabled - remove the checkboxes
				this.$listPane.find("input[type=checkbox]").remove();
				// compensate checkbox offset
				this.$listPane.find(".panel-title").css({
					"padding-right" : "0px"
				});
			}
		};

		/**
		 Filters the list
		 @param {String} filterValue - criterium
		 */
		listSection.prototype.filter = function(filterValue) {
			if (this.controlsVisible) {
				filterValue = filterValue ? filterValue : this.$filterField.val();
				filterValue = utils.cutdown(filterValue);
				// search in panel titles
				var numPanels = this.$listPane.find('.panel').length;
				this.$listPane.find(".panel").each(function() {
					//numPanels++;
					var $el = $(this).find(".filterable");
					var txt = $el.text().toLowerCase();
					if (utils.cutdown(txt).indexOf(filterValue) > -1) {
						$el.parents('.panel').fadeIn(250);
					} else {
						$el.parents('.panel').hide();
					}
				});
				// highlight filter field if not empty
				if (filterValue.length !== 0) {
					this.$filterField.css({
						"background-color" : "#fcf8e3"
					});
				} else {
					this.$filterField.css({
						"background-color" : "white"
					});
				}
				// remove existing warning
				this.$listPane.find(".filterwarning").remove();
				// no items left? notify user that filter is active
				if (!this.$listPane.find(".panel:visible").exists()) {
					this.$listPane.append("<div class='filterwarning alert alert-warning'>" + t("IDS_WEB_FILTER_WARNING", numPanels) + "</div>");
				}
			}
		};

		/**
		 Updates the sections control bar (selections + filter).

		 Note: Call after updateList()
		 */
		listSection.prototype.updateControls = function() {
			if (!this.data || !this.data[this.name] || this.data[this.name].length === 0) {
				this.controlsVisible = false;
				// hide details, selection and filter
				this.$selectionBtn.hide();
				this.$filterField.hide();
				this.$itemDetails.hide();
			} else {
				this.controlsVisible = true;
				this.$selectionBtn.show();
				this.$filterField.show();
				this.$itemDetails.show();
				// update filter
				this.$filterField.trigger('search');
				// update controls
				this.handleSelectionMenu();
			}
		};

		/**
		 Disables an items buttons
		 @param {String} uuid - the items uuid
		 */
		listSection.prototype.disableItem = function(uuid) {
			// disable list item
			this.getItemElementByUuid(uuid).find(".btn").addClass("disabled");
			// disable details if item is selected
			if (uuid === this.selectedItem.uuid) {
				this.disableDetails();
			}
		};

		/**
		 Disables the details pane's buttons
		 */
		listSection.prototype.disableDetails = function() {
	//		this.$itemDetails.find(".btn").addClass("disabled");
		};

		/**
		 Disables the details pane's buttons
		 */
		listSection.prototype.enableDetails = function() {
    //		this.$itemDetails.find(".btn").removeClass("disabled");
		};

		/**
		 Scroll to item.
		 */
		listSection.prototype.focusItem = function() {
			if (!utils.testMediaBreakPoint("xs")) {
				var offset = 0;
				var scope = this;
				this.$listPane.find(".panel").each(function(index) {
					var uuid = $(this).find("a").prop("href");
					uuid = uuid.substring(uuid.lastIndexOf("/") + 1);
					if (uuid === scope.selectedItem.uuid) {
						offset = $(this).offset().top;
						return false;
					}
				});
				offset -= $("#navigation").find(".navbar ").height();
				if (this.$container.find(".scrollpane_control").exists()) {
					offset -= 50;
				}
				// scroll to item
				this.$listPane.scrollTop(offset - 10);
			} else {
				this.focusDetails();
			}
		};

		/**
		 Scroll to details.
		 */
		listSection.prototype.focusDetails = function() {
			// scroll to details (only when collapsed)
			if (utils.testMediaBreakPoint("xs")) {
				$("html, body").scrollTop(this.$itemDetails.find(".panel-title").offset().top - 70);
			}
		};

		/**
		 Creates and inserts a new list item.
		 @param {Object} item - the item's JSO
		 @param {String} title
		 @param {String} hash
		 @returns {Object} $ - the new item
		 */
		listSection.prototype.newItemFromTemplate = function(item, title, hash) {
			// create a new item from template
			var $itemTemplate = $(this.templates.$item.html());
			var $link = $itemTemplate.find(".title_link");
			$link.text(title);
			$link.attr("title", title);
			$link.data("uuid", item.uuid);
			$link.attr("href", hash + item.uuid);
			$link.attr("id", item.uuid);
			// to dom
			this.$listPane.append($itemTemplate);
			// link btn
			$itemTemplate.find("#" + item.uuid).click( function(scope, itm) {
				return function() {
					scope.selectItem(itm);
					scope.pull();
				};
			}(this, item));
			// return the inserted item
			return $itemTemplate.children(":first");
		};

		/**
		 Creates and inserts a new list item.
		 @param {String} classSelector - jQuery class selector for valid items (e.g. '.start_able')
		 @param {String} url - webservice endpoint for the action
		 @param {String} question - question to ask before the action
		 @param {String} question_pl - pluralized version
		 @param {String} process - message to display after action
		 @param {String} process_pl - pluralized version
		 */
		listSection.prototype.selectedAction = function(classSelector, url, question, question_pl, process, process_pl) {
			var scope = this;
			// make sure there is a selection
			if (this.selected.length === 0) {
				return;
			}
			var calls = [];
			var name = "";
			this.$listPane.find(classSelector).each(function(index) {
				var $el = $(this).find("a:first");
				var uuid = $el.attr("id");
				if (scope.selected.indexOf(uuid) > -1) {
					name = $el.text();
					calls.push(uuid);
				}
			});
			if (calls.length == 0) {
				return;
			}
			// confirmation
			var msg = calls.length > 1 ? t(question_pl) : t(question, name);
			utils.confirm(msg, function(result) {
				if (!result) {
					return;
				}
				// build requests
				var requests = [];
				for (var i = 0; i < calls.length; i++) {
					scope.disableItem(calls[i]);
					requests.push(utils.request("/" + scope.name + "/" + calls[i] + url));
				}
				scope.cancelPull(true);
				// deferred cb
				$.when(requests).then(function() {
					var msg = calls.length > 1 ? t(process_pl) : t(process, name);
					app.notifications.success(msg, 2000);
					app.pull();
					scope.clearSelected();
				});
			});
		};

		/**
		 Makes the list section fit the browser viewport.
		 */
		listSection.prototype.resize = function() {
			if (!utils.testMediaBreakPoint("xs")) {
				// desktop two-bar scroll mode
				var height = $(window).height() - $("#footer").height() - $("#navigation").find(".navbar ").height();
				if (this.$container.find(".scrollpane_control").exists()) {
					height -= 50;
				}
				this.$listPane.css("height", height + "px");
				this.$itemDetails.css("height", height + "px");
				this.$container.css("padding-bottom", "0px");
			} else {
				// free willy for mobile (xs)
				this.$listPane.css("height", "100%");
				this.$itemDetails.css("height", "100%");
				this.$container.css("padding-bottom", "60px");
			}
		};

		return listSection;
	})();

application.section.login = (/** @lends application.section.login */
	function() {
		login.prototype = Object.create(application.section.prototype);
		/**
		 Login section (basic section).
		 @extends application.section
		 @constructor
		 */
		function login() {
			// call super
			application.section.call(this);
			// section name
			this.name = "login";
			this.verbose_name = "IDS_WEB_LOGIN";
			// section data url
			this.url = "/login";
			// skip info section in navbar
			this.linkInNavBar = false;
			// $ elements
			this.$form = null;
			this.$formContainer = null;
			this.$formUsername = null;
			this.$formPassword = null;
			this.$formMessage = null;
			this.$globalUserName = null;
			// init
			this.init();
		}

		/**
		 Init - should be called after document load
		 */
		login.prototype.init = function() {
			// call super
			application.section.prototype.init.call(this);
			// elements
			this.$form = this.$container.find("#login-form");
			this.$formContainer = this.$container.find("#login-container");
			this.$formUsername = this.$form.find("#login-form_username");
			this.$formPassword = this.$form.find("#login-form_password");
			this.$formMessage = this.$form.find("#login-form_message");
			this.$globalUserName = $("#footer").find("#global_username");
			// login form
			var scope = this;
			this.$form.submit(function(e) {
				e.preventDefault();
				// validation
				if (scope.$formUsername.val().length === 0) {
					scope.$formMessage.text(t("IDS_WEB_EMPTY_LOGIN_NAME"));
					scope.$formMessage.fadeIn();
					scope.$formContainer.effect("shake", {
						distance : 5,
						times : 2
					}, 500);
					return;
				}
				// login request
				app.spin(true);
				utils.requestPost(scope.url, {
					username : scope.$formUsername.val(),
					password : $.md5(scope.$formPassword.val())
				}, this, function(data) {
					// success
					app.spin(false);
					scope.$formMessage.hide();
					// set user
					if (!app.user) {
						app.user = {};
					}
					app.user.username = data.username;
					app.user.isadmin = data.isadmin;
					app.user.uuid = data.useruuid;
					app.user.language = data.language;
					// for good measure...
					app.storeSession();
					// localization
					app.lang = data.labels;
					// get section hash
					var hash = utils.getCurrentHashWithoutParams();
					// if user logged in on the login page redirect him to the jobs page
					var setHash = false;
					if (hash === "login" || hash === "logout") {
						hash = app.sections.jobs.name;
						setHash = true;
					}
					// check visibility
					app.checkVisibility();
					// display user badge
					scope.showUserName();
					// goto section
					app.gotoSection(hash, setHash);
				}, function(data) {
					app.spin(false);
					if (data.status === 0) {
						scope.$formMessage.text(t("IDS_WEB_SERVERNOTREACHABLEOROFFLINE"));
					} else {
						scope.$formMessage.text(t("IDS_WEB_WRONG_LOGIN_DATA"));
					}
					scope.$formMessage.fadeIn();
					scope.$formContainer.effect("shake", {
						distance : 5,
						times : 2
					}, 500);
				});
			});
		};

		/**
		 Set
		 */
		login.prototype.set = function() {
			// call super
			application.section.prototype.set.call(this);
			// clear and hide user info
			this.$globalUserName.text("").hide();
			// clear form
			this.$formPassword.val("");
			// focus username
			this.$formUsername.focus();
			// clear and hide message
			this.$formMessage.text("").hide();
		};

		/**
		 Show global username
		 */
		login.prototype.showUserName = function() {
			// show user label
			if (app.user) {
				this.$globalUserName.removeClass("hidden").show();
				this.$globalUserName.html(app.user.username + (app.user.isadmin ? " (<span data-trs-translate='IDS_WEB_ADMINISTRATOR'>" + t("IDS_WEB_ADMINISTRATOR") + "</span>)" : ""));
			}
		};

		return login;
	})();

application.section.info = (/** @lends application.section.info */
	function() {
		info.prototype = Object.create(application.ajaxSection.prototype);
		/**
		 Info section (basic section).
		 @extends application.ajaxSection
		 @constructor
		 */
		function info() {
			// call super
			application.ajaxSection.call(this);
			// section name
			this.name = "info";
			this.verbose_name = "IDS_WEB_INFO";
			// section data url
			this.url = "/info";
			// skip info section in navbar
			this.linkInNavBar = true;
			// $ elements
			this.$mem = null;
			this.$os = null;
			this.$version = null;
			this.$cpucount = null;
			this.$osup = null;
			this.$applicationUptime = null;
			this.$applicationTotalBytes = null;
			this.$drive = null;
			// init
			this.init();
		}

		/**
		 Init - should be called after document load
		 */
		info.prototype.init = function() {
			// call super
			application.ajaxSection.prototype.init.call(this);
			// list container
			var $list = this.$container.find(".list-group");
			// get template
			this.templates.$infoItem = $("#info-item_template");
			// memory info
			this.$servicename = $(this.templates.$infoItem.html());
			$list.append(this.$servicename);
			// os info
			this.$os = $(this.templates.$infoItem.html());
			$list.append(this.$os);
			// version info
			this.$version = $(this.templates.$infoItem.html());
			$list.append(this.$version);
			// cpu time
			this.$cpucount = $(this.templates.$infoItem.html());
			$list.append(this.$cpucount);
			// memory info
			this.$mem = $(this.templates.$infoItem.html());
			$list.append(this.$mem);
			// server memory info
			this.$applicationTotalBytes = $(this.templates.$infoItem.html());
			$list.append(this.$applicationTotalBytes);
			// os uptime
			// this.$osup = $(this.templates.$infoItem.html());
			// $list.append(this.$osup);
			// application uptime
			this.$applicationUptime = $(this.templates.$infoItem.html());
			this.$applicationUptime.find(".txt").text(t("IDS_WEB_INFO_UPTIME"));
			$list.append(this.$applicationUptime);
			// drive info
			this.$drive = $(this.templates.$infoItem.html());
			$list.append(this.$drive);
			// add link to navbar branding anchor
			var sectionName = this.name;
			$(".navbar-brand").click(function(e) {
				e.preventDefault();
				app.gotoSection(sectionName, true);
			});
		};

		/**
		 Updates the sections content.
		 */
		info.prototype.update = function() {
			// service name
			this.$servicename.find(".badge").text(this.data.servicename);
			// memory info
			var memTxt = utils.memoryToString(this.data.memory.totalBytes);
			this.$mem.find(".badge").text(memTxt);
			// application memory
			var applicationMemTxt = utils.memoryToString(this.data.memory.applicationTotalBytes);
			this.$applicationTotalBytes.find(".badge").text(applicationMemTxt);
			// os info
			this.$os.find(".badge").text(this.data.os);
			// cpu count and speed
			this.$cpucount.find(".badge").text(this.data.cpucount + "x" + this.data.cpuspeed / 1000 + " GHz");
			// version info
			var versionTxt = "CINEMA 4D R" + this.data.version.toString().substr(0, 2) + "." + this.data.version.toString().substr(2);
			this.$version.find(".badge").text(versionTxt);
			// os up time
			// this.$osup.find(".badge").text(utils.msToStr(this.data.osuptime));
			// application up time
			this.$applicationUptime.find(".badge").text(utils.msToStr(this.data.applicationUptime));
			// drive info
			var driveTxt = t("IDS_WEB_INFO_DRIVETXT", utils.memoryToString(this.data.drive.availableBytes), utils.memoryToString(this.data.drive.totalBytes));
			var $badget = this.$drive.find(".badge").text(driveTxt);
			switch (this.data.drive.status) {
				case "critical":
					$badget.addClass("alert-danger");
					break;
				case "warning":
					$badget.addClass("alert-warning");
					break;
			}
			this.$applicationTotalBytes.find(".txt").text(t("IDS_WEB_INFO_RAMUSAGE"));
			this.$servicename.find(".txt").text(t("IDS_WEB_SERVICE_NAME"));
			this.$mem.find(".txt").text(t("IDS_WEB_INFO_RAM"));
			this.$os.find(".txt").text(t("IDS_WEB_INFO_OS"));
			this.$version.find(".txt").text(t("IDS_WEB_INFO_VERSION"));
			this.$cpucount.find(".txt").text(t("IDS_WEB_INFO_CPU"));
			// this.$osup.find(".txt").text(t("IDS_WEB_INFO_OS_UPTIME"));
			this.$drive.find(".txt").text(t("IDS_WEB_INFO_DRIVE", this.data.drive.drive));
		};

		return info;
	})();

application.section.logout = (/** @lends application.section.logout */
	function() {
		logout.prototype = Object.create(application.section.prototype);
		/**
		 Logout section (basic section).
		 @extends application.section
		 @constructor
		 */
		function logout() {
			// call super
			application.section.call(this);
			// section name
			this.name = "logout";
			this.verbose_name = "IDS_WEB_LOGOUT";
			// init
			this.init();
		}

		/**
		 Init - should be called after document load
		 */
		logout.prototype.set = function() {
			// call super
			application.section.prototype.set.call(this);
			// confirmation dialog
			var msg = t("IDS_WEB_LOGOUT_CONFIRM");
			if (app.runningUploads > 0) {
				msg += " " + (app.runningUploads > 1 ? t("IDS_WEB_JOB_STILL_UPLOADING_MULTI", app.runningUploads) : t("IDS_WEB_JOB_STILL_UPLOADING"));
			}
			// confirm logout
			utils.confirm(msg, function(result) {
				// kill session
				if (result) {
					utils.request("/logout", this, function() {
						app.killSession(true);
					}, function() {
						app.killSession(true);
					});
				}
			});
		};

		return logout;
	})();

application.section.jobs = (/** @lends application.section.jobs */
	function() {
		jobs.prototype = Object.create(application.listSection.prototype);
		/**
		 Jobs section (list section).
		 @extends application.listSection
		 @constructor
		 */
		function jobs() {
			// call super
			application.listSection.call(this);
			// section name
			this.assetscache = enums.DISABLE;
			this.resultassetscache = enums.DISABLE;
			this.name = "jobs";
			this.verbose_name = "IDS_WEB_JOBS";
			// section data url
			this.url = "/jobs";
			// previously selected list item
			this.selectedItemPrev = null;
			// details cache
			this.rawDetailsCache = null;
			// result preview image link
			this.resultPreviewUrl = null;
			// skip update (for sort-dragging)
			this.holdUpdate = false;
			// zip
			this.zipIntervall = null;
			this.zipIntervallTmpValue = 100;
			// $ elements
			this.$selectedStart = null;
			this.$selectedStop = null;
			this.$selectedDelete = null;
			this.$btnAdd = null;
			this.$btnControl = null;
			this.$btnDelete = null;
			this.$previewImg = null;
			this.$previewImgRefresh = null;
			this.$previewImgNoScene = null;
			this.$createDateTime = null;
			this.$renderDateTime = null;
			this.$progress = null;
			this.$userName = null;
			this.$tabs = null;
			this.$message = null;
			this.$log = null;
			this.$results = null;
			this.$assets = null;
			this.$warning = null;
			this.$toUpload = {};
			// init
			this.init();
		}

		jobs.prototype.initValues = function() {
			this.assetscache = enums.DISABLE;
			this.resultassetscache = enums.DISABLE;
		}

		/**
		 Init - should be called after document load.
		 */
		jobs.prototype.init = function() {
			// call super
			application.listSection.prototype.init.call(this);
			// elements
			this.templates.$unitBtns = $("#jobs-details-unitbtns_template");
			this.$selectedStart = this.$container.find(".select_action_start");
			this.$selectedStop = this.$container.find(".select_action_stop");
			this.$selectedDelete = this.$container.find(".select_action_delete");
			this.$btnAdd = this.$container.find("#jobs-details_btn_add");
			this.$btnControl = this.$container.find("#jobs-details_btn_startstop");
			this.$btnDelete = this.$container.find("#jobs-details_btn_delete");
			this.$previewImg = this.$container.find("#jobs-details_preview_img");
			this.$previewImgRefresh = this.$container.find(".jobs-details_preview_btn_refresh");
			this.$previewImgLightbox = this.$container.find(".jobs-details_preview_btn_lightbox");
			this.$previewImgNoScene = this.$container.find("#jobs-details_preview_nodefaultscene");
			this.$dateTimeCreate = this.$container.find("#jobs-details-datetime_create");
			this.$dateTimeStart = this.$container.find("#jobs-details-datetime_start");
			this.$position = this.$container.find("#jobs-details_position");
			this.$progress = this.$container.find("#jobs-details_progress");
			this.$dateTimeRenderTime = this.$container.find("#jobs-details-datetime_rendertime");
			this.$totalFrames = this.$container.find("#jobs-details_totalframes");
			this.$userName = this.$container.find("#jobs-details_username");
			this.$tabs = this.$container.find("#jobdetails_nav");
			this.$log = this.$container.find("#jobs-details_console").find("textarea");
			this.$results = this.$container.find("#jobs-details_results");
			this.$assets = this.$container.find("#jobs-details_assets");
			this.$message = this.$container.find("#jobs-details_message");
			this.$filterTabContent = this.$container.find(".nav .filter");
			
			this.templates.$activeDivider = $('#jobs-item-active-divider_template');
			this.templates.$jobs_placeholder = $("#jobs-placeholder_template");
			// preview
			this.$previewImgRefresh.hide();
			this.$previewImg.parent().hide();
			this.$previewImgNoScene.show();
			// add btn events
			this.addActions();
		};

		/**
		 Register buttons and other dom-events
		 */
		jobs.prototype.addActions = function() {
			// btn events
			var scope = this;
			// add job btn
			this.$btnAdd.click(function() {
				scope.addJob();
			});
			// start / stop btn
			this.$btnControl.click(function() {
				scope.toggleJob(scope.selectedItem, !(scope.selectedItem.id >= 0));
			});
			// delete btn
			this.$btnDelete.click(function() {
				scope.deleteJob(scope.selectedItem);
			});
			// preview image update btn
			this.$previewImgRefresh.click(function() {
				scope.updatePreviewImage(scope.selectedItem);
			});
			// preview image update btn
			this.$previewImgLightbox.click(function() {
				$.colorbox({
					rel : 'jobs-details',
					scalePhotos : true,
					maxWidth : "95%;",
					maxHeight : "95%;",
					html : $('#jobs-details_preview_img').clone().wrap("<div></div>").parent().html(),
					open: true,
					onOpen : function() {
						scope.cancelPull();
						scope.holdUpdate = true;
					},
					onClosed : function() {
						scope.holdUpdate = false;
						scope.pull();
					}
				});
			});
			// list control start
			this.$selectedStart.click(function(e) {
				e.preventDefault();
				scope.selectedAction(".start_able", "/start", "IDS_WEB_JOB_START_CONFIRM", "IDS_WEB_JOB_START_CONFIRM_MULTIPLE", "IDS_WEB_JOB_STARTING", "IDS_WEB_JOB_STARTING_MULTIPLE");
			});
			// list control stop
			this.$selectedStop.click(function(e) {
				e.preventDefault();
				scope.selectedAction(".stop_able", "/stop", "IDS_WEB_JOB_STOP_CONFIRM", "IDS_WEB_JOB_STOP_CONFIRM_MULTIPLE", "IDS_WEB_JOB_STOPPING", "IDS_WEB_JOB_STOPPING_MULTIPLE");
			});
			// list control delete
			this.$selectedDelete.click(function(e) {
				e.preventDefault();
				scope.deleteSelected();
			});
			
			// make the job list sortable
			this.$listPane.sortable({
				axis : "y",
				items : ".dragable",
				placeholder : "sortable-placeholder col-xs-1",
				start : function(event, ui) {
					scope.holdUpdate = true;
					$item = ui.item;
					ui.placeholder.height($item.innerHeight());
					ui.placeholder.width($item.width());
				},
				stop : function(event, ui) {
					scope.holdUpdate = false;
					// find next- or prev. item and send to server
					var $el = $(ui.item);
					var uuid = $el.find("a").attr("href").substring($el.find("a").attr("href").lastIndexOf("/") + 1);
					var nextId = null;
					var prevId = null;
					if ($el.next().length > 0) {
						nextId = $el.next().find("a").attr("href").substring($el.next().find("a").attr("href").lastIndexOf("/") + 1);
						utils.request("/jobs/" + uuid + "/move?insertbefore=" + nextId, this, app.pull);
					} else if ($el.prev().length > 0) {
						prevId = $el.prev().find("a").attr("href").substring($el.prev().find("a").attr("href").lastIndexOf("/") + 1);
						utils.request("/jobs/" + uuid + "/move?insertafter=" + prevId, this, app.pull);
					}
				}
			});
			
			this.$filterTabContent.bind('keyup search',function() {
				$this = $(this);
				var items = scope.$container.find('.assetitem');
				var filter = $this.val();
				if (filter.length === 0) {
					items.show();
				} else {
					items.hide();
					items.each(function() {
						if ($(this).text().indexOf(filter) >= 0) {
							$(this).show();
						} 
					});
				}
			});
			
			this.$container.find('a[data-toggle=tab]').on('show.bs.tab', function(e) {
				if($(e.target).data('filter')) {
					scope.$filterTabContent.prop("disabled", false);
				} else {
					scope.$filterTabContent.prop("disabled", true);
				}
			});
			
			this.$listPane.sortable("disable");
			
		};

		/**
		 Clears the raw JSON cache - forces a redraw on next pull().
		 */
		jobs.prototype.invalidateCache = function() {
			// call super
			application.listSection.prototype.invalidateCache.call(this);
			// clear details cache
			this.rawrawDetailsCache = null;
		};

		/**
		 Pull new data.
		 */
		jobs.prototype.pull = function(timeRunLoop) {
			this.cancelPull();
			app.spin(true);
			var uuid = "";
			// on the first pull, select job ....00001
			if (this.selectedItem) {
				uuid = this.selectedItem.uuid;
			} else {
				uuid = "00000000-0000-0000-0000-000000000001";
			};
			// pull log, assets and results for selected job (along with the list)
			var flag = enums.DETAILSELECTOR_SELECTED;
			var unflag = enums.DETAILSELECTOR_NONE;
			
			var cacheAssets = this.assetscache;
			var cacheResultAssets = this.resultassetscache;

			if (timeRunLoop === undefined || timeRunLoop == false) {
				cacheAssets = false;
				cacheResultAssets = false;
			}

			var url = this.url + "?assets=" + flag + "&results=" + flag + "&log=" + flag + "&rdata=" + unflag + "&rgroups=" + unflag + "&jobid=" + uuid + "&assetscache=" + cacheAssets + "&resultassetscache=" + cacheResultAssets;
			this.requestHandler = utils.request(url, this, [this.setData, this.lazyUpdate], this.pullError);
		};

		/**
		 Update list.
		 */
		jobs.prototype.updateList = function() {
			// skip update while dragging or using the lightbox
			if (this.holdUpdate) {
				return;
			}

			var scope = this;
			// clear list
			this.$listPane.html("");
			// build list
			var numActive = 0;
			for (var i = 0; i < this.data[this.name].length; i++) {
				var item = this.data[this.name][i];
				// create item
				var $itemElement = this.newItemFromTemplate(item, item.name, "#/jobs/");
				// link btn
				$itemElement.find("#" + item.uuid).off().click( function(scope, itm) {
					return function() {
						scope.assetscache = enums.DISABLE;
						scope.resultassetscache = enums.DISABLE;
						scope.selectItem(itm);
					};
				}(this, item));
				// info
				var $status = $itemElement.find(".message");
				var $user = $itemElement.find(".user");
				var $position = $itemElement.find(".position");
				var $progressbar = $itemElement.find(".job_progress_bar .progress-bar");
				var $progressbartxt = $itemElement.find(".job_progress_bar .progress-bar span");
				// status
				if (item.statusshort.length > 0) {
					$status.text(t("IDS_WEB_JOB_" + item.statusshort.toUpperCase()));
				}
				$status.removeClass();
				$status.addClass("message");
				var contextualColor = utils.getJobContextualColor(item);
				if (!contextualColor) {
					$status.addClass("text-info");
				} else {
					$status.addClass("text-" + contextualColor);
				}
				// user
				if (app.user.isadmin) {
					$user.text(item.user);
				} else {
					$user.parent().empty();
				}
				if (item.id) {
					$position.text(item.id);
				} else {
					$position.parent().parent().remove();
					$user.parent().removeClass("col-lg-4").addClass("col-lg-6");
				}
				// progress
				$progressbar.css("width", Math.min(item.progressgui, 100.0) + "%");
				$progressbar.removeClass();
				$progressbar.parent().removeClass();
				$progressbar.parent().addClass("progress");
				$progressbar.addClass("progress-bar");
				if (item.statusshort === "progress" || item.statusshort === "assemble") {
					$progressbar.css('transition-duration', '3s');
				} else {
					$progressbar.css('transition-duration', '0s');
				}
				if (contextualColor === "info") {
					$progressbar.parent().addClass("active");
					if (item.statusshort !== "pending") {
						$progressbar.parent().addClass("progress-striped");
					}
				} else if (contextualColor) {
					$progressbar.addClass("progress-bar-" + contextualColor);
				}
				$progressbartxt.text(String(item.progress * 100.0).substr(0, 4) + "%");
				// update the selection list when an item is selected/deselected
				$itemElement.find("input").click(function()
				{
					scope.setSelected();
				});
				// control btn
				var $btnControl = $itemElement.find(".controlbutton");
				// active ?
				var isActive = item.id >= 0;
				if (isActive) {
					numActive++;
					$btnControl.find(".btn_label").html(" " + t("IDS_WEB_JOB_STOP"));
					$btnControl.find(".glyphicon").removeClass("glyphicon-play").addClass("glyphicon-stop");
				} else {
					$itemElement.find(".glyphicon-sort").hide();
					$btnControl.find(".btn_label").html(" " + t("IDS_WEB_JOB_START"));
					$btnControl.find(".glyphicon").removeClass("glyphicon-stop").addClass("glyphicon-play");
					if (item.statusshort.toLowerCase() === "prepare") {
						$btnControl.addClass("disabled");
					}
				}
				// add event
				$btnControl.click( function(itm, isActive) {
					return function() {
						scope.selectItem(itm);
						scope.toggleJob(itm, !isActive);
					};
				}(item, isActive));
				// set state
				if (!isActive) {
					$itemElement.addClass("inactive");
					$itemElement.addClass("delete_able");
					$itemElement.addClass("start_able");
				} else {
					$itemElement.parent().addClass("dragable");
					$itemElement.addClass("stop_able");
				}
			}
			// enable / disable sorting
			if (numActive <= 1) {
				this.$listPane.sortable("disable");
				// hide sorting-icon
				this.$listPane.find(".glyphicon-sort").hide();
				// compensate title offset for checkbox only
				this.$listPane.find(".panel-title").css({
					"padding-right" : "26px"
				});
			} else {
				this.$listPane.sortable("enable");
				// compensate title offset for checkbox and sort-icon
				this.$listPane.find(".panel-title").css({
					"padding-right" : "46px"
				});
			}
			
			$('div > div.start_able').first().parent().before(this.templates.$activeDivider);
			
			// disable txt selection
			this.$listPane.disableSelection();
			
			this.$listPane.find(".panel.stop_able").bind("mousedown",function() {
				var $this = $(this);
				$this.css('cursor', 'pointer');
				if (scope.$listPane.find("div:not(:first-child) .panel").index(this) !== -1) { 
					scope.$listPane.sortable("enable");
				} else {
					scope.$listPane.sortable("disable");
					$this.css('cursor', 'not-allowed');
				}
			});
			
			this.$listPane.find(".panel.stop_able").bind("mouseup mouseleave",function() {
				$(this).css('cursor', 'pointer');
			});
			
			// call super
			application.listSection.prototype.updateControls.call(this);
		};

		/**
		 Update detail page.
		 @param {Object} item - the JSON data JSO to display
		 */
		jobs.prototype.updateDetails = function(item) {
			//console.profile("Processing pixels");

			// skip update while dragging or using the lightbox
			if (this.holdUpdate) {
				return;
			}

			// check cache
			var data = JSON.stringify(item);
			if (this.rawDetailsCache === data) {
				return;
			} else {
				this.rawDetailsCache = data;
			}
			// call super
			application.listSection.prototype.updateDetails.call(this);
			var scope = this;
			// controls (start/stop)
			this.$btnDelete.removeClass("disabled");
			this.$btnControl.removeClass("disabled");
			if (item.id >= 0) {
				this.$btnControl.find(".btn_label").html(" <span data-trs-translate='IDS_WEB_JOB_STOP'>" + t("IDS_WEB_JOB_STOP") + "</span>");
				this.$btnControl.find(".glyphicon").removeClass("glyphicon-play").addClass("glyphicon-stop");
				this.$btnDelete.addClass("disabled");
			} else {
				if (item.statusshort === "prepare") {
					this.$btnControl.addClass("disabled");
					this.$btnDelete.addClass("disabled");
				}
				this.$btnControl.find(".btn_label").html(" <span data-trs-translate='IDS_WEB_JOB_START'>" + t("IDS_WEB_JOB_START") + "</span>");
				this.$btnControl.find(".glyphicon").removeClass("glyphicon-stop").addClass("glyphicon-play");
			}
			// set details
			this.$itemDetails.addClass()
			this.$itemDetails.find(".panel-title").text(t("IDS_WEB_JOBS_DETAILS") + ": " + item.name);
			this.$dateTimeCreate.text(item.datetime_create !== "" ? item.datetime_create : "-");
			this.$dateTimeStart.text(item.datetime_start !== "" ? item.datetime_start : "-");
			this.$progress.text(String(item.progress * 100).substr(0, 4) + "%");
			if (item.id >= 0) {
				this.$position.text(item.id);
				$("#jobs-details_position-li").show();
			} else {
				$("#jobs-details_position-li").hide();
			}
			if (item.framesrel === item.framesabs) {
				this.$totalFrames.text(item.framesabs > 0 ? t("IDS_WEB_JOB_FRAMESALL", item.framesabs) : "-");	
			} else {
				this.$totalFrames.text(item.framesabs > 0 ? t("IDS_WEB_JOB_FRAMES", item.framesrel, item.framesabs) : "-");	
			}
			this.$dateTimeRenderTime.text(item.datetime_rendertime > 0 ? utils.msToStr(item.datetime_rendertime * 1000) : "-");
			this.$userName.text(item.user);
			// status
			this.$message.text(t("IDS_WEB_JOB_" + item.statusshort.toUpperCase()));
			this.$message.removeClass();
			var contextualColor = utils.getJobContextualColor(item);
			if (!contextualColor) {
				this.$message.addClass("text-info");
			} else {
				this.$message.addClass("text-" + contextualColor);
			}
			// console
			if (item.current_logs) {
				$("#jobs-details_logs-count").text(item.current_logs.length);
				var outp = "";
				for (var j = 0; j < item.current_logs.length; j++) {
					outp += item.current_logs[j] + "\n";
				}
				this.$log.val(outp);
				this.$log.scrollTop(9999999);
			}
			// hide all uploads and show those with matching uuid
			scope.$assets.find(".uploaditem[data-jobuuid!=" + scope.selectedItem.uuid + "]").hide();
			scope.$assets.find(".uploaditem[data-jobuuid=" + scope.selectedItem.uuid + "]").show();
			// assets
			if (item.assets && item.assets_modified) {

				this.$assets.children().each(function(i) {
					var $this = $(this);
					// just delete the entries which were not manually added by the user
					var upload = $this.data("upload");
					if (!upload) {
						$this.remove();
					}
				});

				// update badge count
				$("#jobs-details_assets-count").text(item.assets.length);

				// upload controls
				if (this.$assets.find("#uploadControls").length === 0) {
					var fileSelectorTmpl = "<label for='fileselector' class='uploadlabel btn btn-xs btn-primary'><input type='file' name='fileselector' id='fileselector' multiple class='file_selector'></input><span class='glyphicon glyphicon-plus'></span> " + t("IDS_WEB_JOB_ASSET_SELECT") + "</label>";
					var uploadAllTmpl = "<a href='#' class='btn btn-xs btn-default' id='upload_all'><span class='glyphicon glyphicon-floppy-open'></span> " + t("IDS_WEB_JOB_ASSET_UPLOAD_ALL") + "</a>";
					var $li = $("<div id='uploadControls' class='list-group-item list-group-item-info' upload='1'><div class='btn-group col-md-12'>" + fileSelectorTmpl + uploadAllTmpl + "</div></div>");
					this.$assets.prepend($li);
					// Set OS fileselector width dynamically and hide it to allow custom upload button and localization.
					$("#fileselector").width($("#jobs .uploadlabel").outerWidth());
					// file type input
					this.$assets.find("input").change(function() {
						scope.buildUploadList(item, this.files);
						
						// clear input element - files can not be re-selected otherwise
						$(this).val(null);
					});
					// upload all btn
					this.$assets.find("#upload_all").click(function() {
						scope.uploadAllFiles(scope.selectedItem.uuid, true);
					});
					// check if upload_all btn is available
					this.checkUploadAllBtn();
				};

				if (item.assets.length > 0) {
					var c4dArray = [];

					// push all c4d files to the front of the array
					for (var i = 0; i < item.assets.length; i++) {
						if (!item.assets[i].imagesequence && utils.endsWith(item.assets[i].name, "c4d")) {
							var it = item.assets.splice(i--, 1)[0];
							c4dArray.push(it);
						}
					}

					item.assets = c4dArray.concat(item.assets);

					// add elements
					var $btnDefault = $("<button type='button' class='btn btn-xs btn-default btn-master-scene' style='padding-top:4px; margin-left:5px;'> " + t("IDS_WEB_JOB_ASSET_SET_DEFAULT") + "</button>");

		
					var totalAssetTable = "";
					for (var k = 0; k < item.assets.length; k++) {
						// looking for default scene
						var isDefaultScene = item.defaultscenename === item.assets[k].relpath;

						// build row
						var textFilename = "<div class='assetitem-left col-md-8'>";

						if (item.assets[k].imagesequence) {
							textFilename += item.assets[k].imagesequenceprefix + " <span class='badge badge-default badge-light'>" + item.assets[k].imagesequencefrom + ".." + item.assets[k].imagesequenceto + "</span> " + item.assets[k].imagesequenceappendix;
						} else {
							textFilename += item.assets[k].name;

							// attach the glyphicon or default button to the filename if its a c4d extension
							if (item.assets[k].name.substr(item.assets[k].name.length - 3).toLowerCase() === "c4d") {
								if (isDefaultScene) {
									textFilename += " <span class='glyphicon glyphicon-star' style='margin-left:5px;'></span>";
								} else {
									textFilename += $btnDefault.attr("data-file", item.assets[k].name).attr("data-path", item.assets[k].relpath)[0].outerHTML;
								}
							}
						}

						textFilename += "</div>";

						textFilename += "<button type='button' class='btn btn-sm btn-danger glyphicon glyphicon-trash pull-right delete-asset' data-file='" + item.assets[k].relpath + "' data-defaultscene='" + isDefaultScene + "' data-file='" + item.assets[k].name + "' data-path='" + item.assets[k].relpath + "'></button>";
						
						// add badge-size
						textFilename += "<span class='badge badge-default pull-right badge-size'>" + utils.memoryToString(item.assets[k].size) + "</span>";

						if (item.assets[k].imagesequence) {
							textFilename += "<span class='badge badge-default badge-size'>" + (item.assets[k].imagesequenceto - item.assets[k].imagesequencefrom + 1) + " Files</span>";
						}

						// surround it with an asset row
						totalAssetTable += "<div class='list-group-item list-group-item-custom assetitem'>" + textFilename + "</div>";
					}

					this.$assets.append(totalAssetTable);

					$(".btn-master-scene").click(function(e) {
						var file = $(e.target).data("file");
						var path = $(e.target).data("path");
						scope.setAsDefaultScene(file, path);
					});

					$(".delete-asset").click(function(e) {
						var file = $(e.target).data("file");
						var path = $(e.target).data("path");
						scope.deleteAsset(file, path);
						// update preview if default scene is deleted
						var isDefaultScene = $(e.target).data("defaultscene");
						if (isDefaultScene)
							scope.updatePreviewImage(scope.selectedItem);
					});

				} else {
					this.$assets.append("<div class='list-group-item list-group-item-warning list-group-item-custom col-md-12'><strong>" + t("IDS_WEB_JOB_ASSET_NOT_AVAILABLE") + "</strong></div>");
				}
			}
			// results
			var toAppend = ''
			if (item.resultassets && item.resultassets_modified) {
				this.$results.empty();
				// update badge cnt
				$("#jobs-details_results-count").text(item.resultassets.length);
				this.resultPreviewUrl = null;
				if (item.resultassets.length > 0) {
					// add control bar
					var $controlBar = null;
					if (this.$results.find("#clearresults").length === 0) {
						var prepareZip = "<a href='#' id='preparezip' class='btn btn-xs btn-primary'><span class='glyphicon glyphicon-time'></span> " + t("IDS_WEB_JOB_RESULT_ZIP_PREPARE") + "</a>";
						var progressBar = "<div style='width: 100%; padding-top:0px; margin-bottom: 5px; margin-top: 5px; display: none;' class='progress progress-striped active'><div class='progress-bar' role='progressbar' style='width: " + this.zipIntervallTmpValue + "%; padding-top: 0px; '>" + t("IDS_WEB_JOB_RESULT_ZIP_IN_PROGRESS") + "</div></div>";
						var clearResults = "<a href='#' id='clearresults' class='btn btn-xs btn-danger'><span class='glyphicon glyphicon-trash'></span> " + t("IDS_WEB_JOB_RESULT_CLEAR") + "</a>";
						$controlBar = $("<div class='list-group-item list-group-item-info list-group-item-custom'><div class='btn-group col-md-3'>" + prepareZip + "</div><div class='col-md-5'>" + progressBar + "</div><div class='col-md-4'><div class='btn-group pull-right'>"+ clearResults + "</div></div></div>");
					}

					// results
//	                new assetitem
					var li_template = "<div class='list-group-item list-group-item-custom assetitem'>" +
									"<div class='col-md-8 resultname'>" +
									"<a href='#PREVIEW_URL#' class='btn btn-xs btn-default lightbox'><span class='glyphicon glyphicon glyphicon-picture'></span></a>" +
										"#RESULTNAME#" +
									"</div>" +
									"<div class='btn-group pull-right'>" +
										"<button type='button' class='btn btn-xs btn-danger glyphicon glyphicon-trash' data-file='#DATA_FILE#'></button>" +
										"<a href='#DOWNLOAD_URL#' class='btn btn-xs glyphicon glyphicon-save btn-primary'></a>" +
									"</div>" +
									"<span class='badge badge-default pull-right badge-size'>#SIZE#</span>" +
								"</div>";
					var $btnDownload = $();
					
					var zipIsInProgress = false;
					for (var k = 0; k < item.resultassets.length; k++) {
						var name = item.resultassets[k].name;
						// progress item in list
						if (name === "progress" && !this.zipIntervall) {
							scope.$results.find(".progress").show();
							// indicates the zip progress
							this.zipIntervall = setInterval(function() {
								utils.request("/jobs/" + item.uuid + "/result/zip/progress", this, function(data) {
									scope.$results.find(".progress-bar").css("width", data.progress + "%");
									scope.zipIntervallTmpValue = data.progress;
									if (String(data.progress) === "100") {
										clearInterval(scope.zipIntervall);
										scope.zipIntervall = null;
										scope.zipIntervallTmpValue = 100;
										scope.$results.find(".progress").hide();
									}
								}, function() {
									clearInterval(scope.zipIntervall);
									scope.zipIntervall = null;
									scope.zipIntervallTmpValue = 100;
								});
							}, app.config.timer);
							zipIsInProgress = true;
							continue;
						}
						// new assetitem
						var li = li_template.replace('#RESULTNAME#', name)
						li = li.replace("#SIZE#", utils.memoryToString(item.resultassets[k].size));
						li = li.replace('#DATA_FILE#', name);
						li = li.replace('#DOWNLOAD_URL#', "/jobs/" + item.uuid + "/result/get/" + encodeURIComponent(item.resultassets[k].name) );
						
						// add lightbox for browser compatible image types
						var extension = name.substr((name.lastIndexOf(".") + 1)).toLowerCase();
						if (extension !== "mov" && extension !== "avi" && extension !== "zip") {
							this.resultPreviewUrl = "/jobs/" + item.uuid + "/result/get/" + encodeURIComponent(name) + "?returnjpg=1";
							li = li.replace("#PREVIEW_URL#", this.resultPreviewUrl);
						} else {
							li = li.replace("<a href='#PREVIEW_URL#' class='btn btn-xs btn-default lightbox'><span class='glyphicon glyphicon glyphicon-picture'></span></a>", "");
						}
						
						// to dom - show zip files first
						if (utils.endsWith(name, ".zip"))
							toAppend = li + toAppend;
						else
							toAppend += li;
					}

					if ($controlBar) {
						var prepareZip = "<a href='#' id='preparezip' class='btn btn-xs btn-primary'><span class='glyphicon glyphicon-time'></span> " + t("IDS_WEB_JOB_RESULT_ZIP_PREPARE") + "</a>";
						var clearResults = "<a href='#' id='clearresults' class='btn btn-xs btn-danger'><span class='glyphicon glyphicon-trash'></span> " + t("IDS_WEB_JOB_RESULT_CLEAR") + "</a>";
						var $li = $("<div class='list-group-item list-group-item-info list-group-item-custom assetitem row'><div class='btn-group col-md-3'>" + prepareZip + "</div><div class='col-md-5'>" + progressBar + "</div><div class='col-md-4'><div class='btn-group pull-right'>"+ clearResults + "</div></div></div>");
						this.$results.prepend($controlBar);
						// prepare zip btn
						this.$results.find("#preparezip").click(function() {
							$(this).addClass("disabled");
							scope.prepareZipArchive();
						});
						this.$results.find("#clearresults").click(function() {
							scope.clearAllResults();
						});
					}

					if (zipIsInProgress) {
						scope.$results.find(".progress").show();
					} else {
						clearInterval(this.zipIntervall);
						scope.zipIntervall = null;
						scope.zipIntervallTmpValue = 100;
						scope.$results.find(".progress").hide();
					}

				} else {
					toAppend += "<div class='col-md-12 list-group-item list-group-item-custom list-group-item-warning'><strong>" + t("IDS_WEB_JOB_RESULT_NOT_AVAILABLE") + "</div>";
				}
				var $tmp_results = $(toAppend);
				this.$results.append($tmp_results);
				// delete btn
				$tmp_results.find('button').click(function(e) {
					var file = $(e.target).data("file");
					scope.deleteResult(file);
				});
				$tmp_results.find('.lightbox').colorbox({
					rel : "Results",
					photo: true,
					scalePhotos : true,
					maxWidth : "95%;",
					maxHeight : "95%;",
					onOpen : function() {
						scope.cancelPull();
						scope.holdUpdate = true;
					},
					onClosed : function() {
						scope.holdUpdate = false;
						scope.pull();
					}
					
				})
			}
			// this is a non-detailed data set? if so, pull details
			if (!item.assets || !item.resultassets) {
				this.$tabs.find("btn").addClass("disabled");
				this.$tabs.fadeTo(200, 0.5);
				this.disableDetails();
				// try to pull the preview (also clears residual preview)
				this.updatePreviewImage(item);
				this.pull();
				return;
			} else {
				// pull the preview image at first load of a new item...
				if (this.selectedItemPrev !== this.selectedItem) {
					// update previously selected list item
					this.selectedItemPrev = this.selectedItem;
					// update img
					this.updatePreviewImage(item);
				}
			}
			this.$tabs.fadeTo(200, 1);
			this.$tabs.find("btn").removeClass("disabled");

			this.assetscache = enums.ENABLE;
			this.resultassetscache = enums.ENABLE;
		};

		/**
		 Update the preview image.
		 @param {Object} item - the JSON data JSO
		 */
		jobs.prototype.updatePreviewImage = function(item) {
			var d = new Date();
			var url = item ? "/jobs/" + item.uuid + "/documentpreview?" + d.getTime() : "";
			// get img
			var scope = this;
			this.$previewImg.error(function() {
				scope.$previewImgRefresh.hide();
				scope.$previewImg.parent().hide();
				scope.$previewImgNoScene.show();
			}).load(function() {
				scope.$previewImgRefresh.show();
				scope.$previewImg.parent().show();
				scope.$previewImgNoScene.hide();
			}).attr("src", url);
		};

		/**
		 Adds a new job.
		 */
		jobs.prototype.addJob = function() {
			var scope = this;
			utils.prompt(t("IDS_WEB_JOB_NEW_QUESTION"), function(result) {
				if (!result) {
					return;
				}
				var jobName = $.trim(result);
				// check if job name is valid and does not already exist
				if (jobName.length === 0) {
					app.notifications.danger(t("IDS_WEB_JOB_NEW_NAME_NOT_EMPTY"));
					return;
				}
				if (!jobName.match(/^[a-zA-Z0-9_. -]+$/)) {
					app.notifications.danger(t("IDS_WEB_JOB_NEW_RESTRICTION"));
					return;
				}
				for (var i = 0; i < scope.data.jobs.length; i++) {
					if (utils.cutdown(scope.data.jobs[i].name) === utils.cutdown(jobName)) {
						app.notifications.danger(t("IDS_WEB_JOB_NEW_EXISTS", jobName));
						return;
					}
				}
				// send to server
				utils.requestPost("/jobs/create", {
					jobname : jobName
				}, this, function() {
					// deselect
					scope.selectedItem = null;
					app.notifications.success(t("IDS_WEB_JOB_CREATED", jobName), 2000);
					app.pull();
				});
			});
		};

		/**
		 Starts or stops a render job.
		 @param {Object} job - the job JSO
		 @param {Boolean} start - true for start
		 */
		jobs.prototype.toggleJob = function(job, start) {
			var scope = this;
			if (start) {
				scope.disableItem(job.uuid);
				scope.cancelPull(true);
				app.notifications.success(t("IDS_WEB_JOB_STARTING", job.name), 2000);
				utils.request("/jobs/" + job.uuid + "/start", this, app.pull);
			} else {
				utils.confirm(t("IDS_WEB_JOB_STOP_CONFIRM", job.name), function(result) {
					if (!result) {
						return;
					}
					scope.disableItem(job.uuid);
					scope.cancelPull(true);
					app.notifications.success(t("IDS_WEB_JOB_STOPPING", job.name), 2000);
					// send to server
					utils.request("/jobs/" + job.uuid + "/stop", this, app.pull);
				});
			}
		};

		/**
		 deletes a render job.
		 @param {Object} job - the job JSO
		 */
		jobs.prototype.deleteJob = function(job) {
			//confirmation dialog
			var scope = this;
			utils.confirm(t("IDS_WEB_JOB_DELETE_CONFIRM", scope.selectedItem.name), function(result) {
				if (!result) {
					return;
				}
				scope.disableItem(job.uuid);
				scope.cancelPull(true);
				app.notifications.success(t("IDS_WEB_JOB_DELETING", job.name), 2000);
				// send to server
				utils.request("/jobs/" + job.uuid + "/delete", this, app.pull);
			});
		};

		/**
		 Adds the given files as uploadable entry to the asset list.
		 @param {Object} job - the job JSO
		 @param {Array} files - the file list
		 */
		jobs.prototype.buildUploadList = function(job, files) {
			var $defaultHook = this.$assets.find("#uploadControls:nth-child(1)");
			var file = null;
			var existing = null;
			var uploadCount = 0;
			var scope = this;

			var $div = $("<div class='col-md-4'></div>");
			$div.css("padding-right", "0px");
			
			// walk files
			
			for (var i = 0; i < files.length; i++) {
				var file = files[i];
				//file = files[i];
				if (file.size > 1024 * 1024 * 1024 * 4) {
					app.notifications.danger(t("IDS_WEB_UPLOADEDFILETOOBIG", file.name, 4));
					continue;
				}
				// ensure that a file is not added twice to the list
				var filenameExistsInUploadList = false;				
				if (scope.$assets.find("[data-file='" +  utils.cutdown(file.name)+"']").length > 0) {
					filenameExistsInUploadList = true;
					existing = file.name;
				};
				// ensure that file is not already in asset list
				var filenameExistsInAssetList = false;
				for (var j = 0; j < job.assets.length; j++) {
					if (job.assets[j].name && utils.cutdown(job.assets[j].name) === utils.cutdown(file.name)) {
						filenameExistsInAssetList = true;
						break;
					}
				}
				if (filenameExistsInUploadList || filenameExistsInAssetList) {
					existing = file.name;
					continue;
				}
				// build row
	
				var $progressbar = $("<div class='progress col-md-3'><div class='progress-bar' role='progressbar'><span>0%</span></div></div>");
				var $cancelbtn = $("<button type='button' class='btn btn-xs btn-danger hidden'><span class='glyphicon glyphicon-stop'></span> " + t("IDS_WEB_JOB_ASSET_CANCEL") + "</button>");
				var $removebtn = $("<button type='button' class='btn btn-xs btn-danger cancelbtn'><span class='glyphicon glyphicon glyphicon-floppy-remove'></span> " + t("IDS_WEB_JOB_ASSET_REMOVE") + "</button>");
				var $uploadbtn = $("<button type='button' class='btn btn-xs btn-default uploadbtn'><span class='glyphicon glyphicon-floppy-open'></span> " + t("IDS_WEB_JOB_ASSET_UPLOAD") + "</button>");
				var $newFile = $("<div data-jobuuid='" + job.uuid + "' data-upload='true' class='list-group-item uploaditem'><span id='filename' class='col-md-5'>" + files[i].name + "</span></div>");
				// to dom
				$newFile.append($progressbar);
				var $tmpDiv = $("<div class='btn-group pull-right'></div>");
				$newFile.append($tmpDiv);
				$tmpDiv.append($uploadbtn);
				$tmpDiv.append($cancelbtn);
				$tmpDiv.append($removebtn);
				$tmpDiv.wrap($div);
				$defaultHook.after($newFile);
				// btns
				$removebtn.click( function(scope, $newFile) {
					return function() {
						$newFile.remove();
						scope.checkUploadAllBtn();
					};
				}(this, $newFile));
				
				$uploadbtn.click( function(scope, itm, uuid, $progressbar, $newFile, $uploadbtn, $cancelbtn, $removebtn) {
					return function() {
						scope.uploadFile(itm, uuid, $newFile, $progressbar.children(":first"), $uploadbtn, $cancelbtn, $removebtn);
					};
				}(scope, file, job.uuid, $progressbar, $newFile, $uploadbtn, $cancelbtn, $removebtn));
				// counter
				uploadCount++;
				
			}	
				
			// double file error
			if (existing) {
				app.notifications.danger(t("IDS_WEB_JOB_ASSET_FILE_EXISTS", existing));
			}
			this.checkUploadAllBtn();
		};

		/**
		 Uploads a file.
		 @param {Object} file - the file
		 @param {String} file - job uuid
		 @param {Object} $newFile - new file $
		 @param {Object} $progressbar - progressbar $
		 @param {Object} $uploadbtn - upload btn $
		 @param {Object} $cancelbtn - cancel btn $
		 @param {Object} $removebtn - remove btn $
		 */
		jobs.prototype.uploadFile = function(file, jobId, $newFile, $progressbar, $uploadbtn, $cancelbtn, $removebtn) {
			// store old click handler
			var jobId = jobId;
			var scope = this;
			// form data obj
			var fd = new FormData();
			fd.append("file", file);
			// visibility
			$cancelbtn.show();
			$uploadbtn.hide();
			$removebtn.hide();
			$progressbar.removeClass("progress-bar-danger");
			this.checkUploadAllBtn();
			// uploader
			var $upload = $.ajax({
				type : "POST",
				processData : false,
				dataType : "json",
				cache : false,
				url : "/jobs/" + jobId + "/asset/add",
				data : fd,
				contentType : false,
				xhr : function() {
					var xhr = new window.XMLHttpRequest();
					xhr.upload.addEventListener("progress", function(evt) {
						if (evt.lengthComputable) {
							var percentComplete = evt.loaded / evt.total;
							$progressbar.css("width", percentComplete * 100.0 + "%");
							if (evt.loaded === evt.total && file.name.substr((file.name.lastIndexOf(".") + 1)).toLowerCase() === "zip") {
								$progressbar.find("span").text(t("IDS_WEB_JOB_ASSET_UNZIPPING"));
							} else {
								$progressbar.find("span").text(Math.round(percentComplete * 10000.0) / 100.0 + "%");
							}
						}
					}, false);
					return xhr;
				},
				complete : function(data) {
					if (!data.status === 0) {
						$uploadbtn.hide();
					}
					$uploadbtn.removeClass("disabled");
					$cancelbtn.hide();
					$removebtn.show();
					scope.checkUploadAllBtn();
					// decrement uploads
					app.runningUploads--;
					// upload all files case
					if (scope.uploadAll && app.runningUploads == 0) {
						$uploadbtn.removeClass("toUpload");
						scope.uploadAllFiles(jobId)
					}
				},
				success : function(data) {
					app.pull();
					scope.updatePreviewImage(scope.selectedItem);
					$newFile.remove();
				},
				error : function(data) {
					$progressbar.addClass("progress-bar-danger");
					var errorMessage = data.responseJSON && data.responseJSON.error ? data.responseJSON.error : data.responseText;
					$progressbar.text(utils.HTML2Text(errorMessage));
					$uploadbtn.show();
					if (!data.status === 0) {
						$cancelbtn.off("click").click(function() {
							$newFile.remove();
						});
					}
				}
			});
			// increment uploads
			app.runningUploads++;
			// cancel btn
			$cancelbtn.on("click").click(function() {
				$upload.abort();
				$uploadbtn.show();
				$removebtn.show();
				$cancelbtn.hide();
			});
		};
		/**
		 Uploads all files to a given job
		 @param {UUID} jobuuid - the job uuid
		 @param {Bool} initial - should only be set to true in case the upload is triggerd using the upload all button.
		 */
		jobs.prototype.uploadAllFiles = function(jobuuid, initial) {
			this.uploadAll = true
			if (jobuuid && initial)
			{
				this.$toUpload[jobuuid] = this.$assets.find(".uploaditem[data-jobuuid=" + jobuuid + "] .uploadbtn:visible")
				this.$toUpload[jobuuid].addClass("toUpload");
			}
			this.$toUpload[jobuuid].each(function() {
				$(this).click();
				if (app.runningUploads > 10) {
					return false;
				}
			});
			if (this.$assets.find(".uploaditem[data-jobuuid=" + jobuuid + "] .uploadbtn.toUpload").length === 0) {				
				this.uploadAll = false;
				delete this.$toUpload[jobuuid];
			}
		}

		/**
		 Sets the given asset as default scene.
		 @param {String} file - the asset's filename
		 */
		jobs.prototype.setAsDefaultScene = function(file, path) {
			var scope = this;
			// confirmation
			utils.confirm(t("IDS_WEB_JOB_ASSET_SET_DEFAULT_CONFIRM", file), function(result) {
				if (!result) {
					return;
				}
				utils.requestPost("/jobs/" + scope.selectedItem.uuid + "/setdefaultscenename", {
					filename : path
				}, app, function() {
					app.notifications.success(t("IDS_WEB_JOB_ASSET_SET_DEFAULT_CHANGED", file), 2000);
					// update preview image after a default scene has changed
					scope.updatePreviewImage(scope.selectedItem);
					app.pull();
				});
			});
		};

		/**
		 Hides/unhides the 'upload all' button if there are any 'upload' buttons.
		 */
		jobs.prototype.checkUploadAllBtn = function() {
			if (!this.$assets.find(".uploadbtn:visible").exists()) {
				this.$assets.find("#upload_all").hide();
			} else {
				this.$assets.find("#upload_all").show();
			}
		};

		/**
		 Deletes the given asset.
		 @param {String} file - the asset's filename
		 */
		jobs.prototype.deleteAsset = function(file, path) {
			var scope = this;
			// confirmation
			utils.confirm(t("IDS_WEB_JOB_ASSET_DELETE_CONFIRM", file), function(result) {
				if (!result) {
					return;
				}
				app.notifications.success(t("IDS_WEB_JOB_ASSET_DELETE_PROCESS", file), 2000);
				utils.requestPost("/jobs/" + scope.selectedItem.uuid + "/asset/delete", {
					filename : path
				}, app, function() {
					// update preview image after a file is deleted
					scope.updatePreviewImage(scope.selectedItem);
					app.pull();
				});
			});
		};

		/**
		 Deletes the given result.
		 @param {String} file - the results's filename
		 */
		jobs.prototype.deleteResult = function(file) {
			var scope = this;
			// confirmation
			utils.confirm(t("IDS_WEB_JOB_RESULT_DELETE_CONFIRM", file), function(result) {
				if (!result) {
					return;
				}
				app.notifications.success(t("IDS_WEB_JOB_RESULT_DELETE_PROCESS", file), 2000);
				// send to server
				utils.requestPost("/jobs/" + scope.selectedItem.uuid + "/result/delete", {
					filename : file
				}, app, app.pull);
			});
		};

		/**
		 Prepares a ZIP archive of all results.
		 */
		jobs.prototype.prepareZipArchive = function(file) {
			// send to server
			utils.request("/jobs/" + this.selectedItem.uuid + "/result/zip", this, app.pull);
			this.$results.find(".progress").show();
		};

		/**
		 Deletes all results.
		 */
		jobs.prototype.clearAllResults = function(file) {
			var scope = this;
			// confirmation
			utils.confirm(t("IDS_WEB_JOB_RESULT_CLEAR_ALL"), function(result) {
				if (!result) {
					return;
				}
				app.notifications.success(t("IDS_WEB_JOB_RESULT_CLEAR_ALL_PROCESS", scope.selectedItem.name), 2000);
				// send to server
				utils.request("/jobs/" + scope.selectedItem.uuid + "/clear", app, app.pull);
			});
		};

		/**
		 Deletes the selected jobs.
		 */
		jobs.prototype.deleteSelected = function() {
			var scope = this;
			// make sure there is a selection
			if (this.selected.length === 0) {
				return;
			}
			var calls = [];
			var name = "";
			var warn = false;
			// are running job in the selection? if so, notify user
			scope.$listPane.find(".isactive").each(function(index) {
				var $el = $(this).find("a:first");
				var uuid = $el.attr("id");
				if (scope.selected.indexOf(uuid) > -1) {
					warn = true;
					return false;
				}
			});
			// delete only selected inactive jobs
			scope.$listPane.find(".delete_able").each(function(index) {
				var $el = $(this).find("a:first");
				var uuid = $el.attr("id");
				if (scope.selected.indexOf(uuid) > -1) {
					name = $el.text();
					calls.push(uuid);
				}
			});
			if (calls.length == 0) {
				return;
			}
			// confirmation
			var msg = calls.length > 1 ? t("IDS_WEB_JOB_DELETE_CONFIRM_MULTIPLE") : t("IDS_WEB_JOB_DELETE_CONFIRM", name);
			utils.confirm(msg, function(result) {
				if (!result) {
					return;
				}
				if (warn) {
					app.notifications.danger(t("IDS_WEB_JOB_STOP_BEFORE_DELETE"));
				}
				// build requests
				var requests = [];
				for (var i = 0; i < calls.length; i++) {
					scope.disableItem(calls[i]);
					requests.push(utils.request("/jobs/" + calls[i] + "/delete"));
				}
				// clear selection
				scope.cancelPull(true, true);
				// deferred cb
				$.when(requests).then(function() {
					var msg = calls.length > 1 ? t("IDS_WEB_JOB_DELETING_MULTIPLE") : t("IDS_WEB_JOB_DELETING", name);
					app.notifications.success(msg, 2000);
					app.pull();
				});
			});
		};

		return jobs;
	})();

application.section.network = (/** @lends application.section.network */
	function() {
		network.prototype = Object.create(application.listSection.prototype);
		/**
		 Network section (list section).
		 @extends application.section
		 @constructor
		 */
		function network() {
			// call super
			application.listSection.call(this);
			// section name
			this.verbose_name = "IDS_WEB_NETWORK";
			this.name = "network";
			// section data url
			this.url = "/network";
			// $ elements
			this.$selectedRestart = null;
			this.$consoleNetwork = null;
			this.$plugins = null;
			this.$description = null;
			this.$status = null;
			this.$log = null;
			this.$logCounter = null;
			this.$logBtn = null;
			this.$infoBtn = null;
			this.$restartBtn = null;
			this.$controls = null;
			this.$restartClientsBtn = null;
			this.$restartServerBtn = null;
			// init
			this.init();
		}

		/**
		 Init - should be called after document load.
		 */
		network.prototype.init = function() {
			// call super
			application.listSection.prototype.init.call(this);
			// elements
			this.$selectedRestart = this.$container.find(".select_action_restart");
			this.$consoleNetwork = this.$itemDetails.find("#network-details_console").find("textarea");
			this.$plugins = this.$itemDetails.find("#network-details_plugins");
			this.$description = this.$itemDetails.find("#network-details_description");
			this.$status = this.$itemDetails.find("#network-details_status");
			this.$log = this.$itemDetails.find("#network-details_log_container");
			this.$logCounter = this.$itemDetails.find("#network-details_log_count");
			this.$logBtn = this.$itemDetails.find("#network-details_log_btn");
			this.$infoBtn = this.$container.find("#network-details_btn_info");
			this.$controls = this.$itemDetails.find("#network-details_controls");
			this.$restartBtn = this.$controls.find("#network-details_restart_btn");
			this.$restartClientsBtn = this.$container.find("#network-controls_restartall");
			this.$restartServerBtn = this.$container.find("#network-controls_restarserver");
			
			this.templates.$activeDivider = $('#network-item-active-divider_template');
			// add btn events
			this.addActions();
		};

		/**
		 Register buttons and other dom-events
		 */
		network.prototype.addActions = function() {
			// btn events
			var scope = this;
			// server info btn
			this.$infoBtn.click(function() {
				app.gotoSection(app.sections.info.name);
			});
			// restart clients btn (admin)
			this.$restartClientsBtn.click(function() {
				scope.restartClients();
			});
			// restart server btn
			this.$restartServerBtn.click(function() {
				scope.restartServer();
			});
			// restart client btn (details) (admin)
			this.$restartBtn.click(function() {
				scope.restartClient();
			});
			// list control restart
			this.$selectedRestart.click(function(e) {
				e.preventDefault();
				scope.selectedAction(".restart_able", "/restart", "IDS_WEB_NETWORK_RESTART_CLIENT_CONFIRM", "IDS_WEB_NETWORK_RESTART_SELECTED_CONFIRM_MULTIPLE", "IDS_WEB_NETWORK_RESTART_CLIENT_PROGRESS", "IDS_WEB_NETWORK_RESTART_SELECTED_PROGRESS_MULTIPLE");
			});
		};

		/**
		 Update list.
		 */
		network.prototype.updateList = function() {
			// clear list
			var scope = this;
			
			scope.$listPane.html("");
			// build list
			for (var i = 0; i < scope.data[this.name].length; i++) {
				var item = scope.data[this.name][i];
				// create item
				var $itemElement = scope.newItemFromTemplate(item, item.name, "#/network/");
				// set machine info
				$itemElement.find(".message").text(t("IDS_WEB_NETWORK_" + item.statusshort.toUpperCase()));
				$itemElement.find(".address").text(item.address);
				if (item.selected == false && item.islocal == false) {
					$itemElement.addClass("excluded");
				}
				// flag delete_able if it's not the user itself
				if (app.user.uuid !== item.uuid) {
					$itemElement.addClass("delete_able");
				}
				// set restart_able if not offline
				if (item.status !== "machinestate_offline") {
					scope.$listPane.children().last().addClass("restart_able");
				}
			}
			$('div > div.excluded').first().parent().before(this.templates.$activeDivider);
			// hide checkboxes if no selection is available (no admin)
			if (!app.user.isadmin) {
				scope.$listPane.find('input:checkbox').hide();
			}
			// update the selection list when an item is selected/deselected
			scope.$listPane.find("input").click(function()
			{
				scope.setSelected();
			});
		};

		/**
		 Update detail page.
		 @param {Object} item - the JSON data JSO to display
		 */
		network.prototype.updateDetails = function(item) {
			// call super
			application.listSection.prototype.updateDetails.call(this);
			// set details
			this.$itemDetails.find(".panel-title").text(t("IDS_WEB_NETWORK_DETAILS") + ": " + item.name);
			// status
			this.$status.text(t("IDS_WEB_NETWORK_" + item.statusshort.toUpperCase()));
			// info
			this.$description.text(item.description);
			this.$plugins.text(item.plugins);
			// hide plugin ingormation if machine is offline
			if (item.status === "machinestate_offline") {
				this.$plugins.parents('.list-group-item').hide();
			} else {
				this.$plugins.parents('.list-group-item').show();
			}
			// is admin ?
			if (app.user.isadmin) {
				// console
				this.$logCounter.text(item.current_logs.length);
				var outp = "";
				for (var j = 0; j < item.current_logs.length; j++) {
					outp += item.current_logs[j] + "\n";
				}
				this.$consoleNetwork.val(outp);
				this.$consoleNetwork.scrollTop(9999999);
				// download log btn
				this.$logBtn.attr({
					target : "_blank",
					href : "/network/" + item.uuid + "/log?logcount=50000"
				});
				// disable download log btn if no log available
				if (item.current_logs.length === 0) {
					this.$logBtn.addClass("disabled");
				} else {
					this.$logBtn.removeClass("disabled");
				}
				// hide control and log for offline machines
				if (item.status === "machinestate_offline") {
					this.$log.hide();
					this.$logBtn.parent().hide();
					this.$controls.hide();
				} else {
					this.$log.show();
					this.$logBtn.parent().show();
					this.$controls.show();
				}
			}
		};

		/**
		 Hide elements not visible to current user level.
		 */
		network.prototype.checkVisibility = function() {
			// no admin?
			if (!app.user.isadmin) {
				this.$selectionBtn.remove();
				this.$filterField.remove();
				this.$logBtn.parent().remove();
				this.$controls.remove();
				this.$restartClientsBtn.remove();
				this.$restartServerBtn.remove();
				this.$log.remove();
				this.$listPane.find('input:checkbox').hide();
			}
		};

		/**
		 Restarts all clients.
		 */
		network.prototype.restartClients = function() {
			var scope = this;
			// confirm action
			utils.confirm(t("IDS_WEB_NETWORK_RESTART_CLIENTS_CONFIRM"), function(result) {
				if (!result) {
					return;
				}
				scope.cancelPull();
				// send to server
				utils.request("network/restart/clients", this, function() {
					app.pull();
					app.notifications.success(t("IDS_WEB_NETWORK_RESTART_CLIENTS_PROGRESS"), 2000);
				});
			});
		};

		/**
		 Restarts the selected client.
		 */
		network.prototype.restartServer = function() {
			var scope = this;
			// confirm action
			utils.confirm(t("IDS_WEB_NETWORK_RESTART_SERVER_CONFIRM"), function(result) {
				if (!result) {
					return;
				}
				scope.cancelPull();
				// send to server
				utils.request("network/restart", this, function() {
					app.pull();
					app.notifications.success(t("IDS_WEB_NETWORK_RESTART_SERVER_PROGRESS"), 2000);
				});
			});
		};
		
		/**
		 Restart the selected machines.
		 */
		network.prototype.restartSelected = function() {
			var scope = this;
			// make sure there is a selection
			if (this.selected.length === 0) {
				return;
			}
			
		};

		/**
		 Restarts the selected client.
		 */
		network.prototype.restartClient = function() {
			var scope = this;
			// confirm action
			utils.confirm(t("IDS_WEB_NETWORK_RESTART_CLIENT_CONFIRM", scope.selectedItem.name), function(result) {
				if (!result) {
					return;
				}
				// deselect
				scope.cancelPull();
				// send to server
				utils.request("network/" + scope.selectedItem.uuid + "/restart", this, function() {
					app.notifications.success(t("IDS_WEB_NETWORK_RESTART_CLIENT_PROGRESS", scope.selectedItem.name), 2000);
					app.pull();
				});
			});
		};

		return network;
	})();

application.section.users = (/** @lends application.section.users */
	function() {
		users.prototype = Object.create(application.listSection.prototype);
		/**
		 Users section (list section).
		 @extends application.section
		 @constructor
		 */
		function users() {
			// call super
			application.listSection.call(this);
			// section name
			this.verbose_name = "IDS_WEB_USER";
			this.name = "users";
			// section data url
			this.url = "/users";
			// form data
			this.formData = {};
			// elements
			this.$form = null;
			this.$btnSave = null;
			this.$btnAdd = null;
			this.$btnPassword = null;
			this.$btnDelete = null;
			this.$formUsername = null;
			this.$formPassword = null;
			this.$formIsAdmin = null;
			this.$formDescription = null;
			// init
			this.init();
		}

		/**
		 Init - should be called after document load.
		 */
		users.prototype.init = function() {
			// call super
			application.listSection.prototype.init.call(this);
			// elements
			this.$form = this.$container.find("#users_form");
			this.$selectedDelete = this.$container.find(".select_action_delete");
			this.$btnSave = this.$container.find("#users-details_btn_save");
			this.$btnAdd = this.$container.find("#users-details_btn_add");
			this.$btnPassword = this.$container.find("#users-details_btn_password");
			this.$btnDelete = this.$container.find("#users-details_btn_delete");
			this.$controls = this.$container.find(".users-details_controls");
			this.$formUsername = this.$container.find("#user-details_form_username");
			this.$formPassword = this.$container.find("#user-details_form_password");
			this.$formIsAdmin = this.$container.find("#user-details_form_isadmin");
			this.$formUserLanguage = this.$container.find("#user-details_form_language");
			this.$formDescription = this.$container.find("#user-details_form_description");
			// add button events
			this.addActions();
		};

		/**
		 Register buttons and other dom-events
		 */
		users.prototype.addActions = function() {
			// btn events
			var scope = this;
			// change
			this.$formPassword.bind("keyup change", function() {
				scope.checkForm();
			});
			this.$formIsAdmin.change(function() {
				scope.checkForm();
			});
			this.$formUserLanguage.change(function() {
				scope.checkForm();
			});
			this.$formDescription.bind("keyup change", function() {
				scope.checkForm();
			});
			// add user (admin)
			this.$btnAdd.click(function() {
				scope.addUser();
			});
			// delete user (admin)
			this.$btnDelete.click(function(e) {
				e.preventDefault();
				scope.removeUser();
			});
			// submit (admin)
			this.$form.submit(function(e) {
				e.preventDefault();
				scope.submitUserDetails();
			});
			// reset password
			this.$btnPassword.click(function() {
				scope.resetPassword();
			});
			// language dropdown
			this.$formUserLanguage.change(function() {
				scope.setLanguage();
			});
			// list control delete (admin)
			this.$selectedDelete.click(function(e) {
				e.preventDefault();
				scope.deleteSelected();
			});
		};

		/**
		 Checks if the form data was changed.
		 */
		users.prototype.checkForm = function() {
			// get state
			var tmpPassword = this.$formPassword.val();
			var tmpIsAdmin = this.$formIsAdmin.prop("checked");
			var tmpDescription = this.$formDescription.val();
			// compare
			var data = this.formData;
			if (tmpPassword !== data.password || tmpIsAdmin !== data.isadmin || tmpDescription !== data.description) {
				this.$btnSave.removeClass("disabled");
			} else {
				this.$btnSave.addClass("disabled");
			}
		};

		/**
		 Update list.
		 */
		users.prototype.updateList = function() {
			var scope = this;
			// clear list
			this.$listPane.html("");
			// build list
			for (var i = 0; i < this.data[this.name].length; i++) {
				var item = this.data[this.name][i];
				// create item
				var $itemElement = this.newItemFromTemplate(item, item.username, "#/users/");
				// set user type
				$itemElement.find(".type").html(item.isadmin ? "<span data-trs-translate='IDS_WEB_ADMINISTRATOR'>" + t("IDS_WEB_ADMINISTRATOR") + "</span>" : "<span data-trs-translate='IDS_WEB_ADMINISTRATOR'>" + t("IDS_WEB_USER") + "</span>");
				// flag delete_able if it's not the user itself
				if (app.user.uuid !== item.uuid) {
					$itemElement.addClass("delete_able");
				}
				// update the selection list when an item is selected/deselected
				$itemElement.find("input").click(function()
				{
					scope.setSelected();
				});
			}
			// hide checkboxes if no selection is available (no admin)
			if (!app.user.isadmin) {
				this.$listPane.find('input:checkbox').hide();
			}
		};

		/**
		 Update detail page.
		 @param {Object} item - the JSON data JSO to display
		 */
		users.prototype.updateDetails = function(item) {
			// call super
			application.listSection.prototype.updateDetails.call(this);
			// set details
			this.$itemDetails.find(".panel-title").html("<span data-trs-translate='IDS_WEB_USER_DETAILS'>" + t("IDS_WEB_USER_DETAILS") + "</span>: " + item.username);
			// store form data
			this.formData.password = "";
			this.formData.isadmin = item.isadmin;
			this.formData.description = item.description;
			// set form data
			this.$formUsername.text(item.username);
			this.$formIsAdmin.prop("checked", item.isadmin);
			this.$formUserLanguage.parent().hide();
			this.$btnSave.addClass("disabled");
			// admin may not edit himself
			if (app.user.username === this.selectedItem.username) {
				this.$formUserLanguage.empty();
				for (var i = 0, length = app.languages.length; i < length; i++) {
					var current_lang = app.languages[i];
					if (app.user && current_lang.extensions.toLowerCase() == app.user.language.toLowerCase()) {
						this.$formUserLanguage.append("<option selected='selected' value='" + current_lang.extensions + "'>" + current_lang.name + "</option>");
					} else {
						this.$formUserLanguage.append("<option value='" + current_lang.extensions + "'>" + current_lang.name + "</option>");
					}
				}
				this.$formUserLanguage.parent().show();
			}
			if (app.user.isadmin) {
				// hide elements if admin is viewing himself
				if (app.user.username === this.selectedItem.username) {
					this.$formDescription.parent().hide();
					this.$formPassword.parent().hide();
					this.$formIsAdmin.prop("disabled", true);
					this.$controls.hide();
				} else {
					this.$formDescription.val(item.description);
					this.$formDescription.parent().show();
					this.$formPassword.val("");
					this.$formPassword.parent().show();
					this.$formIsAdmin.prop("disabled", false);
					this.$controls.show();
				}
			} else {
				// disable 'is admin' checkbox for users
				this.$formIsAdmin.prop("disabled", true);
			}
			if (app.user.username === this.selectedItem.username) {
				this.$btnPassword.show();
			} else {
				this.$btnPassword.hide();
			}
		};

		/**
		 Hide elements not visible to current user level.
		 */
		users.prototype.checkVisibility = function() {
			// is admin?
			if (!app.user.isadmin) {
				this.$selectionBtn.remove();
				this.$filterField.remove();
				this.$btnSave.remove();
				this.$btnAdd.remove();
				this.$controls.remove();
				this.$formDescription.parent().remove();
				this.$formPassword.parent().remove();
			} else {
				this.$btnAdd.show();
			}
		};

		/**
		 Adds a new user (admin only)
		 */
		users.prototype.addUser = function() {
			var scope = this;
			// create user CB
			var createUserFn = function(username, password, isadmin) {
				scope.cancelPull();
				// submit to server
				utils.requestPost("/users/create", {
					username : username,
					password : $.md5(password),
					isadmin : isadmin,
					description : ""
				}, this, function() {
					scope.selectedItem = null;
					app.notifications.success(t("IDS_WEB_USER_ADD_OK", username), 2000);
					app.pull();
				});
			};
			// prompt for username
			utils.prompt(t("IDS_WEB_USER_ADD_NAME"), function(result) {
				if (result === null) {
					return;
				}
				var username = $.trim(result);
				// check if username is given...
				if (username.length === 0) {
					app.notifications.danger(t("IDS_WEB_USER_ADD_NAME_EMPTY"));
					return;
				}
				// valid...
				if (!username.match(/^[a-zA-Z0-9_.-]+$/)) {
					app.notifications.danger(t("IDS_WEB_USER_ADD_NAME_ILLEGAL"));
					return;
				}
				// and does not already exist
				for (var i = 0; i < scope.data.users.length; i++) {
					if (utils.cutdown(scope.data.users[i].username) === utils.cutdown(username)) {
						app.notifications.danger(t("IDS_WEB_USER_ADD_NAME_EXISTS", username));
						return;
					}
				}
				// prompt for the new user's password
				utils.prompt(t("IDS_WEB_USER_ADD_PWD", username), function(result) {
					if (result === null) {
						return;
					}
					var password = result;
					if (password.length === 0) {
						app.notifications.danger(t("IDS_WEB_USER_PWD_EMPTY"));
						return;
					}
					// create user as admin?
					bootbox.dialog({
						message : t("IDS_WEB_USER_ADD_ADMIN", username),
						buttons : {
							cancel : {
								label : t("IDS_WEB_CANCEL"),
								className : "btn-default"
							},
							danger : {
								label : t("IDS_WEB_POS"),
								className : "btn-danger",
								callback : function() {
									createUserFn(username, password, true);
								}
							},
							main : {
								label : t("IDS_WEB_NEG"),
								className : "btn-primary",
								callback : function() {
									createUserFn(username, password, false);
								}
							}
						}
					});
				});
			});
		};

		/**
		 Deletes a user (admin only)
		 */
		users.prototype.removeUser = function() {
			var scope = this;
			// confirm action
			utils.confirm(t("IDS_WEB_USER_DEL", this.selectedItem.username), function(result) {
				if (!result) {
					return;
				}
				var username = scope.selectedItem.username;
				// submit to server
				scope.cancelPull(true);
				scope.disableDetails();
				utils.request("/users/" + scope.selectedItem.uuid + "/delete", this, function() {
					app.notifications.success(t("IDS_WEB_USER_DEL_OK", username), 2000);
					app.pull();
				});
			});
		};

		/**
		 Submits a change to a user's details (admin only)
		 */
		users.prototype.submitUserDetails = function() {
			var scope = this;
			// confirm edit
			utils.confirm(t("IDS_WEB_USER_EDIT_CONFIRM", scope.selectedItem.username), function(result) {
				if (!result) {
					return;
				}
				// check for changed fields
				var newpassword = undefined;
				if (scope.$formPassword.val() !== scope.formData.password && scope.$formPassword.val() !== "") {
					newpassword = $.md5(scope.$formPassword.val());
				}
				var isadmin = undefined;
				if (scope.$formIsAdmin.prop("checked") !== scope.formData.isadmin) {
					isadmin = scope.$formIsAdmin.is(":checked");
				}
				var newdescription = undefined;
				if (scope.$formDescription.val() !== scope.formData.description) {
					newdescription = scope.$formDescription.val();
				}
				var postData = {
					useruuid : scope.selectedItem.uuid,
					newpassword : newpassword,
					newisadmin : isadmin,
					newdescription : newdescription
				};
				// submit to server
				utils.requestPost("/users/edit", postData, this, function() {
					app.notifications.success(t("IDS_WEB_USER_EDIT_CHANGED", scope.selectedItem.username), 2000);
					app.pull();
				});
			});
		};

		/**
		 Resets the current user's password
		 */
		users.prototype.resetPassword = function() {
			// promp for current password
			utils.prompt(t("IDS_WEB_USER_CHANGE_PWD_OLD"), function(result) {
				if (result == null) {
					return;
				}
				var oldpassword = result;
				if (oldpassword.length === 0) {
					app.notifications.danger(t("IDS_WEB_USER_CHANGE_PWD_MTY"));
					return;
				}
				// test current password is valid
				utils.requestPost("/settings/password/verify", {
					password : $.md5(oldpassword)
				}, this, function(data) {
					if (!data.result) {
						app.notifications.danger(t("IDS_WEB_WRONG_PASSWORD"));
						return;
					}
					// prompt new password
					utils.prompt(t("IDS_WEB_USER_CHANGE_PWD_NEW"), function(result) {
						if (result == null) {
							return;
						}
						var newpassword = result;
						if (newpassword.length === 0) {
							app.notifications.danger(t("IDS_WEB_USER_CHANGE_PWD_MTY"));
							return;
						}
						// verify-prompt new password
						utils.prompt(t("IDS_WEB_USER_CHANGE_PWD_NEW_2"), function(result) {
							if (result == null) {
								return;
							}
							if (result !== newpassword) {
								app.notifications.danger(t("IDS_WEB_USER_CHANGE_PWD_NOK"));
								return;
							}
							// passwords match - confirm one more time
							utils.confirm(t("IDS_WEB_USER_CHANGE_PWD_SAVE"), function(result) {
								if (!result) {
									return;
								}
								// submit to server
								utils.requestPost("/settings/password/set", {
									useruuid : app.user.uuid,
									oldpassword : $.md5(oldpassword),
									newpassword : $.md5(newpassword)
								}, this, function() {
									app.pull();
									app.notifications.success(t("IDS_WEB_USER_CHANGE_PWD_OK"), 2000);
								});
							});
						});
					});
				});
			});
		};

		/**
		 Switches the users default language
		 */
		users.prototype.setLanguage = function() {
			var extension = this.$formUserLanguage.val();
			// load localization
			utils.requestPost("/settings/language/set", {
				"language" : this.$formUserLanguage.val()
			}, this, function(data) {
				app.user.language = extension;
				app.lang = data.labels;
				// save a temp copy of the default language as fallback
				if (app.lang_default == null) {
					app.lang_default = app.lang;
				}
				app.storeSession();
				// translate templates
				window.location.reload();
			});
		};

		/**
		 Deletes all selected users (admin only)
		 */
		users.prototype.deleteSelected = function() {
			var scope = this;
			// make sure there is a selection
			if (this.selected.length === 0) {
				return;
			}
			var calls = [];
			var name = "";
			// find all selected, delete_able items
			this.$listPane.find(".delete_able").each(function(index) {
				var $el = $(this).find("a:first");
				var uuid = $el.attr("href");
				uuid = uuid.substring(uuid.lastIndexOf("/") + 1);
				// prevent admins from deleting themselves
				if (scope.selected.indexOf(uuid) > -1) {
					name = $el.text();
					if (uuid === app.user.uuid) {
						app.notifications.danger(t("IDS_WEB_USER_DEL_SELF"));
					} else {
						calls.push(uuid);
					}
				}
			});
			if (calls.length == 0) {
				return;
			}
			// confirmation
			var msg = calls.length > 1 ? t("IDS_WEB_USER_DEL_SELECTED_MULTIPLE") : t("IDS_WEB_USER_DEL", name);
			utils.confirm(msg, function(result) {
				if (!result) {
					return;
				}
				var requests = [];
				// build requests
				for (var i = 0; i < calls.length; i++) {
					scope.disableItem(calls[i]);
					requests.push(utils.request("/users/" + calls[i] + "/delete"));
				}
				// deselect
				scope.disableDetails();
				scope.cancelPull(true);
				// deferred cb
				$.when(requests).then(function(data) {
					var msg = calls.length > 1 ? t("IDS_WEB_USER_DEL_OK_MULTIPLE") : t("IDS_WEB_USER_DEL_OK", name);
					app.notifications.success(msg, 2000);
					app.pull();
				});
			});
		};

		return users;
	})();
application.section.monitor = (/** @lends application.section.monitor */
		function() {
		monitor.prototype = Object.create(application.ajaxSection.prototype);
		/**
		Monitor section (basic section).
		@extends application.ajaxSection
		@constructor
		*/
		function monitor() {
		// call super
		application.ajaxSection.call(this);
		// section name
		this.verbose_name = "IDS_WEB_MONITOR";
		this.name = "monitor";
		// section data url
		this.url = "/monitor";
		// $ elements
		this.$jobsTable = null;
		this.$networksTable = null;
		this.drive = null;
		this.$warning = null;
		this.$noJobs = null;
		// init
		this.init();
		}

		/**
		Init - should be called after document load
		*/
		monitor.prototype.init = function() {
			// call super
			application.ajaxSection.prototype.init.call(this);
			// template
			this.templates.$rowJobItem = $("#monitor_jobtable_row-template table tbody");
			this.templates.$rowNetworkItem = $("#monitor_networktable_row-template table tbody");
			// $ elements
			this.$jobsTable = this.$container.find("#table-jobs");
			this.$networksTable = this.$container.find("#table-network");
			this.$noJobs = this.$container.find(".alert-no-jobs");
			// sorter
			
			var scope = this;

			this.updateControls();
			// set visible
			this.checkVisibility();
			
			this.$container.find('a[data-toggle="pill"]').bind('shown.bs.tab', function (e) {
				scope.url = "/" + $(e.target).attr("target"); 
				scope.pull();
				scope.updateControls();
			});
			
		};

		/**
		Updates the sections content.
		*/
		monitor.prototype.update = function() {
			var scope = this;
			this.$jobsTable.find("tbody tr").remove();
			// build list
			//this.$container.find("#monitor-job-count").text(this.data.jobs.length)
			//this.$container.find("#monitor-machine-count").text(this.data.network.length)
			
			if (this.data.jobs.length == 0) {
				this.$noJobs.show();
				this.$jobsTable.hide();
			} else {
				this.$noJobs.hide();
				this.$jobsTable.show();
			}
			
			for (var i = 0; i < this.data.jobs.length; i++) {
				var item = this.data.jobs[i];
				var contextualColor = utils.getJobContextualColor(item);
				// data
				var $item = $(this.templates.$rowJobItem.html());
				if (contextualColor){
					$item.addClass(contextualColor);
				}
				// link btn col 2
				var $btn = $item.find("a");
				$btn.text(item.name);
				$btn.data("uuid", item.uuid);
				$btn.attr("href", "#/jobs/" + item.uuid);
				$btn.attr("id", "joblink-" + item.uuid);
				$btn.click( function(itm, scope) {
					return function(e) {
						app.sections.jobs.selectedItem = itm;
						app.gotoSection(app.sections.jobs.name);
					};
				}(item, this));
				
				$btnStart = $("<a class='btn btn-primary btn-xs'><i class='glyphicon glyphicon-play'></i></a>");
				$btnStop = $("<a class='btn btn-primary btn-xs'><i class='glyphicon glyphicon-stop'></i></a>");
				$btnClearResults = $("<a class='btn btn-danger btn-xs'><i class='glyphicon glyphicon-trash'></i></a>");
				$($item.find("td.cell-position")).find('.position').text(item.id >= 0 ? item.id : "");
				$($item.find("td.cell-scene")).text(item.defaultscenename);
				$($item.find("td.cell-user")).text(item.user);
				$($item.find("td.cell-created")).text(item.datetime_create);
				$($item.find("td.cell-started")).text(item.datetime_start === "" ? "---" : item.datetime_start);
				$($item.find("td.cell-clients")).text(item.involvedmachines);
				
				$item.find(".badge").text(t("IDS_WEB_JOB_" + item.statusshort.toUpperCase()));
				var $progressbar = $item.find(".progress-bar");
				$progressbar.parent().removeClass();
				$progressbar.parent().addClass("progress");
				$progressbar.removeClass();
				$progressbar.addClass("progress-bar");
				if (contextualColor === "info") {
					$progressbar.parent().addClass("active");
					$progressbar.parent().addClass("progress-striped");
					$($item.find("td.cell-action")).append($btnStop);
					$btnStop.click( function(itm, isActive) {
						return function() {
							scope.toggleJob(itm, isActive);
						};
					}(item, false));
				} else if (contextualColor) {
					$progressbar.addClass("progress-bar-" + contextualColor);
				} 
				if (contextualColor != "info" && contextualColor != "success" && item.defaultscenename !== "") {
					$($item.find("td.cell-action")).append($btnStart);
					$btnStart.click( function(itm, isActive) {
						return function() {
							scope.toggleJob(itm, isActive);
						};
					}(item, true));
				} else if (contextualColor === "success") {
					$($item.find("td.cell-action")).append($btnClearResults);
					$btnClearResults.click( function(itm, isActive) {
						return function() {
							scope.clearAllResults(itm);
						};
					}(item, false));
				}
				$progressbar.css("width", item.progressgui + "%");
				$progressbar.find(".percentage").text(item.progressgui);
				this.$jobsTable.append($item);
			}
			scope.$jobsTable.find('tbody').sortable({
				items: 'tr.info',
				helper: function(e, tr)  {
					var $tds = tr.find('td');
					var $helper = tr.clone();
					$helper.find('td').each(function(idx) {
						$(this).width($tds.eq(idx).outerWidth());
					});
					return $helper;
				},
				start: function() {
					scope.holdUpdate = true;
				},
				stop: function(e, ui) {
					scope.holdUpdate = false;
					var $el = $(ui.item);
					var uuid = $el.find("a").attr("href").substring($el.find("a").attr("href").lastIndexOf("/") + 1);
					var nextId = null;
					var prevId = null;
					if ($el.next().length > 0) {
						nextId = $el.next().find("a").attr("href").substring($el.next().find("a").attr("href").lastIndexOf("/") + 1);
						utils.request("/jobs/" + uuid + "/move?insertbefore=" + nextId, this, app.pull);
					} else if ($el.prev().length > 0) {
						prevId = $el.prev().find("a").attr("href").substring($el.prev().find("a").attr("href").lastIndexOf("/") + 1);
						utils.request("/jobs/" + uuid + "/move?insertafter=" + prevId, this, app.pull);
					}
				}
			});
			this.$jobsTable.bind( 'sortEnd',function(e){
				var th = scope.$jobsTable.find('.tablesorter-headerAsc');
				if ( $(th).index() == 0  && $('tr.info').length > 1) {
					scope.$jobsTable.addClass("jobs-sortable");
					scope.$jobsTable.find('tr.info').bind('mousedown', function () {
						if ($(this).index() == 0) {
							scope.$jobsTable.find('tbody').sortable('disable');
						} else {
							scope.$jobsTable.find('tbody').sortable('enable');
						}
					});
				} else {
					scope.$jobsTable.removeClass("jobs-sortable");
				}
			});
			this.$networksTable.find("tbody tr").remove();
			for (var i = 0; i < this.data.network.length; i++ ) {
				var item = this.data.network[i];
				var $item = $(this.templates.$rowNetworkItem.html());
				
				var contextualColor = utils.getMachineContextualColor(item);
				$item.addClass(contextualColor);
				var n = 0;
				if (item.islocal) {
					var $server = $($item.find("td.cell-selected"))
					$server.find("input").toggleClass("hidden");
					$server.find("span").toggleClass("hidden");
				} else {
					$($item.find("td.cell-selected")).find("input").prop("checked", item.selected);
				}
				$($item.find("td.cell-description")).text(item.description);
				// link btn col 4
				var $btn = $item.find(".cell-machinename a");
				$btn.text(item.name);
				$btn.data("uuid", item.uuid);
				$btn.attr("href", "#/networks/" + item.uuid);
				$btn.attr("id", "machinelink-" + item.uuid);
				$btn.click( function(itm, scope) {
					return function(e) {
						e.preventDefault();
						app.sections.network.selectedItem = itm;
						app.sections.network.selectedItemFirst = true;
						app.gotoSection(app.sections.network.name);
					};
				}(item, this));
				n++;
				$($item.find("td.cell-os")).text(item.os);
				$($item.find("td.cell-status")).text(t("IDS_WEB_NETWORK_" + item.statusshort.toUpperCase()));
				var framestart = item.current_framestart
				if (item.statusshort == "busy" && framestart >= 0) {
					var framestart = item.current_framestart
					var frameend = item.current_frameend
					if (framestart == frameend) {
						$($item.find("td.cell-rendering")).text(item.current_job + " [" + framestart + "]");
					} else {
						$($item.find("td.cell-rendering")).text(item.current_job + " [" + framestart + " - " + frameend + "]");
					}
				}
				this.$networksTable.append($item);
			}
		
			var $table = this.$container.find(".table");
			// update tablesorter
			$table.trigger("update");
			$table.trigger("search",false);
				
		};
		
		/**
		 Makes the section fit the browser viewport.
		 */
		monitor.prototype.resize = function() {
			if (utils.testMediaBreakPoint("lg")) {
				this.$container.find(".tab-pane").addClass("active");
				// desktop two-bar scroll mode
				var height = $(window).height() - $("#footer").height() - $("#navigation").find(".navbar ").height();
				height -= 100;
				this.$container.find('.table-container').css("max-height", height + "px");
				this.$container.find('.table-container').css("overflow", "auto");
				this.$container.css("padding-bottom", "0px");
			} else {
				// free willy for mobile (xs)
				this.$container.find('.table-container').css("height", "100%");
				this.$container.css("padding-bottom", "60px");
			}
		};
		
		
		monitor.prototype.updateControls = function() {
			// update filter restrictor
			var $filterFields = this.$container.find(".btn-group-filter");
			scope = this;
			
			for (var i = 0; i < $filterFields.length; i++ ) {
				field = $filterFields[i];
				$item = $(field);
				var $table = scope.$container.find($item.data("filter-for"));
				var $dropdown = $item.find(".column-selector .dropdown-menu");
				var $filterField = $item.find('.filter');
				
				
				var all = $dropdown.find("[data-value='all']");
				
				$dropdown.find("[data-value!='all']").parents('li').remove();
				$item.find(".column-selector button .text").text(all.text());
				$filterField.val('');
				$filterField.attr("data-column", all.data("value"));
				$table.find("th").each(function(idx) {
					if ($(this).hasClass("filterable")) {
							$dropdown.append("<li><a href='javascript:void(0)' data-value='" + idx +"'>" + $(this).find("span").text() +"</a></li>");
					}
				});
				
				
				$table.tablesorter({
					sortReset : true,
					widthFixed : true,
					sortList: [[0,0]],
					widgets : ["saveSort", "filter"],
					widgetOptions : {
						filter_hideFilters : false,
						filter_anyMatch : false,
						filter_columnFilters : false,
						filter_external: $filterField,
						filter_searchDelay: 0
					}
				});
				
				// update tablesorter
				$table.trigger("update");
				$table.trigger("search", false);
			}
			this.addActions();
		}
		
		monitor.prototype.addActions = function() {
			var scope = this;
			this.$columnSelectors = this.$container.find(".column-selector li a");
			this.$columnSelectors.click(function() {
				$this = $(this);
				$group = $this.parents('.btn-group-filter');
				var column = $this.data("value");
				$group.find(".filter").attr("data-column", column);
				$group.find(".column-selector button .text").text($this.text());
				scope.$container.find($group.data('filter-for')).trigger("search", false);
			});
			
			this.$container.find("[data-toggle=toggle-tab]").on('click', function(e) {
				e.preventDefault();
				$this = $(this);
				href = $this.attr('href');
				$this.toggleClass("active");
				$(href).toggle($this.hasClass("active"));
				active = scope.$container.find(".tab-content .tab-pane.active:visible");
				if (active.length === 1) {
					active.removeClass("col-lg-6");
				} else {
					active.addClass("col-lg-6");
				}
				
			});
			
		}


		/**
		 Pull new data.
		 */
		monitor.prototype.pull = function() {
			this.cancelPull();
			app.spin(true);
			var uuid = "";
			// on the first pull, select job ....00001
			if (this.selectedItem) {
				uuid = this.selectedItem.uuid;
			} else {
				uuid = "00000000-0000-0000-0000-000000000001";
			};
			// pull log, assets and results for selected job (along with the list)
			var flag = enums.DETAILSELECTOR_SELECTED;
			var unflag = enums.DETAILSELECTOR_NONE;
			var url = this.url + "?assets=" + flag + "&results=" + unflag + "&log=" + unflag + "&rdata=" + unflag + "&rgroups=" + unflag + "&jobid=" + uuid;
			this.requestHandler = utils.request(url, this, [this.setData, this.lazyUpdate], this.pullError);
		};

		/**
		 Starts or stops a render job.
		 @param {Object} job - the job JSO
		 @param {Boolean} start - true for start
		 */
		monitor.prototype.toggleJob = function(job, start) {
			var scope = this;
			if (start) {
					scope.cancelPull(true);
					app.notifications.success(t("IDS_WEB_JOB_STARTING", job.name), 2000);
					utils.request("/jobs/" + job.uuid + "/start", this, app.pull);
			} else {
				utils.confirm(t("IDS_WEB_JOB_STOP_CONFIRM", job.name), function(result) {
					if (!result) {
						return;
					}
					scope.cancelPull(true);
					app.notifications.success(t("IDS_WEB_JOB_STOPPING", job.name), 2000);
					// send to server
					utils.request("/jobs/" + job.uuid + "/stop", this, app.pull);
				});
			}
		};
		
		/**
		 Deletes all results.
		 */
		monitor.prototype.clearAllResults = function(job) {
			var scope = this;
			// confirmation
			utils.confirm(t("IDS_WEB_JOB_RESULT_CLEAR_ALL"), function(result) {
				if (!result) {
					return;
				}
				app.notifications.success(t("IDS_WEB_JOB_RESULT_CLEAR_ALL_PROCESS", job.name), 2000);
				// send to server
				utils.request("/jobs/" + job.uuid + "/clear", app, app.pull);
			});
		};
		/**
		 Hide elements not visible to current user level.
		 */
		monitor.prototype.checkVisibility = function() {
			
		};
		
		return monitor;
}	)();

utilities = (/** @lends utilities */
	function() {
		/**
		 Helpers - utilities can be accessed from anywhere via the 'window.utils' handle.
		 @constructor
		 */
		function utilities() {
			// set handy global reference (singleton)
			if (window.utils) {
				utils.log("'utilities' can only be initialized once!");
			} else {
				// utils will be used exclusively from now on to simplify things
				window.utils = this;
			}
			// translation helper ref
			window.t = this.getTranslatedStringFromID;
			// extend jQuery
			jQuery.fn.exists = function() {
				return this.length > 0;
			};
		}

		/**
		 Log to console.
		 @param {String} msg
		 @param {Boolean} force - forces console output in non-debug version
		 */
		utilities.prototype.log = function(msg, force) {
			if (app.config.debug || force) {
				console.log(msg);
			}
		};

		/**
		 Simplified Ajax GET request.
		 @param {String} url
		 @param {Object} scope - for cb
		 @param {Object} callback - success callback - function or array of functions
		 @param {Object} errorcallback - error callback - function or array of functions
		 @param {Function} errorcallback - error callback
		 */
		utilities.prototype.request = function(url, scope, callback, errorcallback, ajaxParams) {
			return utilities.prototype.ajaxRequest(url, null, scope, "GET", callback, errorcallback, ajaxParams);
		};

		/**
		 Simplified Ajax POST request.
		 @param {String} url
		 @param {Object} scope - for cb
		 @param {Object} callback - success callback - function or array of functions
		 @param {Object} errorcallback - error callback - function or array of functions
		 @param {Boolean} no401 - supress auth error handling
		 @param {Function} errorcallback - error callback
		 */
		utilities.prototype.requestPost = function(url, data, scope, callback, errorcallback, ajaxParams) {
			return utilities.prototype.ajaxRequest(url, data, scope, "POST", callback, errorcallback, ajaxParams);
		};

		/**
		 Simplified Ajax request.
		 @param {String} url
		 @param {Object} scope - for cb
		 @param {String} type - "POST "
		 @param {Object} callback - success callback - function or array of functions
		 @param {Object} errorcallback - error callback - function or array of functions
		 @param {Boolean} no401 - supress auth error handling
		 @param {Function} errorcallback - error callback
		 */
		utilities.prototype.ajaxRequest = function(url, data, scope, type, callback, errorcallback, ajaxParams) {
			type = type || "POST";
			// error cb

			var no401 = undefined;

			var tmperrorcallback = function(e, status, error) {
				if (e.statusText !== "abort") {
					var sessionLost = false;
					// catch all 401 and handle login
					if (e.status === 401 && !no401) {
						if (app.user) {
							sessionLost = true;
							// kill session
							app.killSession(false);
							utils.alert(t("IDS_WEB_SESSION_LOST"), function() {
								window.location.reload();
							});
						}
					} else {
						// general error msg - don't show if custom error CB was given
						if (!errorcallback) {
							utils.throwHTTPError(e, status, error, url);
						}
					}
					//call custom error CB
					if (errorcallback && !sessionLost) {
						errorcallback(e, status, error, url);
					}
				}
			};

			var cache = ajaxParams === undefined ? false : ajaxParams.cache;

			// request
			return $.ajax({
				timeout : app.config.timeout,
				url : url,
				type : type,
				data : data,
				datatype : "json",
				success : function(data) {
					if (Object.prototype.toString.call(callback) === "[object Array]") {
						for (var i = 0; i < callback.length; i++) {
							if ( typeof callback[i] === "function") {
								callback[i].call(scope, data);
							}
						}
					} else {
						if ( typeof callback === "function") {
							callback.call(scope, data);
						}
					};
				},
				fail : tmperrorcallback,
				error : tmperrorcallback,
				cache : cache
			});
		};

		/**
		 Shows a HTTP request error.
		 @param {Object} e
		 @param {Object} status
		 @param {Object} error
		 @param {Object} url
		 */
		utilities.prototype.throwHTTPError = function(e, status, error, url) {
			// get error msg
			var errorMessage = "";
			if (e.responseJSON && e.responseJSON.error) {
				errorMessage = e.responseJSON.error;
			} else if (e.responseText) {
				errorMessage = e.responseText;
			}
			// notify user
			var msg = "";
			if (app.config.debug || (e.status !== 0 && e.status !== 404)) {
				msg = "ERROR: (" + e.status + ") :" + utils.HTML2Text(errorMessage) + " (" + url + ")";
			} else {
				msg = t("IDS_WEB_NOCON");
			}
			app.notifications.danger(msg, app.config.timer * 0.75);
			utils.log(msg);
		};

		/**
		 Returns just the url hash.
		 @returns {String}
		 */
		utilities.prototype.getCurrentHashWithoutParams = function(str) {
			var re = new RegExp("#/[a-zA-Z0-9]*");
			var results = location.hash.match(re);
			if (results && results.length > 0) {
				results = results[0].substr(2);
			}
			if (results)
				return utils.cutdown(results);
			else
				return null;
		};

		/**
		 Sets a cookie
		 @param {String} name
		 @param {String} value
		 @param {Number} days - ...to expire
		 */
		utilities.prototype.setCookie = function(name, value, days) {
			if (days) {
				var date = new Date();
				date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
				var expires = "; expires=" + date.toGMTString();
			} else {
				var expires = "";
			}
			document.cookie = app.config.cookiePrefix + name + "=" + value + expires + "; path=/";
		};

		/**
		 Reads a cookie
		 @param {String} name
		 @returns {String} value
		 */
		utilities.prototype.getCookie = function(name) {
			var nameEQ = app.config.cookiePrefix + name + "=";
			var ca = document.cookie.split(";");
			for (var i = 0; i < ca.length; i++) {
				var c = ca[i];
				while (c.charAt(0) === " ")
				c = c.substring(1, c.length);
				if (c.indexOf(nameEQ) === 0) {
					return c.substring(nameEQ.length, c.length);
				}
			}
			return null;
		};

		/**
		 Deletes a cookie
		 @param {String} name
		 */
		utilities.prototype.removeCookie = function(name) {
			this.setCookie(name, "", null);
		};

		/**
		 Walks the given $ element and translates all children with the 'data-trs-translate' attribute.
		 @param {Object} $
		 */
		utilities.prototype.translateElements = function($toTranslate) {
			if (app.lang && $toTranslate) {
				var matches = $toTranslate.find("[data-trs-translate]");
				if (matches.length > 0) {
					for (var i = 0, length = matches.length; i < length; i++) {
						var $match = $(matches[i]);
						try {
							var trans_id = $match.data("trs-translate");
							value = app.lang[trans_id];
							if ( typeof value === "undefined") {
								value = app.lang_default[trans_id];
								if ( typeof value === "undefined") {
									value = "localization unavailable: " + trans_id;
								}
							}
						} catch (e) {
							value = "replace content error: " + e.message;
						}
						if ($match.attr("placeholder")) {
							$match.attr("placeholder", value);
						} else {
							$match.empty();
							$match.append(value);
						}
						// expand
					}
				}
			}
		};

		/**
		 Returns a translated string via the given id string and optional arguments
		 @param {String} id
		 @returns {String}
		 */
		utilities.prototype.getTranslatedStringFromID = function(id) {
			if (app.lang && id) {
				value = app.lang[id];
				if (!value) {
					value = app.lang_default[id];
					if (!value) {
						return "localization unavailable: " + id;
					}
				}
				for (var i = 1, length = arguments.length; i < length; i++) {
					value = value.replace("#", arguments[i]);
				}
				return value;
			}
		};

		/**
		 Wrapper for bootboxjs 'confirm' for custom labeling.
		 @param {String} message - The message of the dialog to be alerted.
		 @param {Function} fn - The callback function.
		 */
		utilities.prototype.alert = function(message, fn) {
			// bootbox
			this.confirm(message, fn);
			// alert hack
			$(".modal-dialog .btn-default").hide();
		};

		/**
		 Wrapper for bootboxjs 'prompt' for custom labeling.
		 @param {String} title - The title of the dialog to be prompted.
		 @param {Function} fn - The callback function.
		 */
		utilities.prototype.prompt = function(title, fn) {
			// bootbox
			bootbox.prompt(title, fn);
			// localization hack
			$(".modal-dialog .btn-primary").text(t("IDS_WEB_POS"));
			$(".modal-dialog .btn-default").text(t("IDS_WEB_CANCEL"));
		};

		/**
		 Wrapper for bootboxjs 'confirm' for custom labeling.
		 @param {String} message - The message of the dialog to be confirmed.
		 @param {Function} fn - The callback function.
		 */
		utilities.prototype.confirm = function(message, fn) {
			// bootbox
			bootbox.confirm(message, fn);
			// localization hack
			$(".modal-dialog .btn-primary").text(t("IDS_WEB_POS"));
			$(".modal-dialog .btn-default").text(t("IDS_WEB_CANCEL"));
		};

		/**
		Checks if a string ends with a specific string.
		@param {String} str - String to check
		@param {String} suffix - The suffix to check.
		*/
		utilities.prototype.endsWith = function(str, suffix) {
			var pos = str.indexOf(suffix);
			return str.substr(pos, str.length - pos) === suffix;
		};

		/**
		 Returns a jquery selector name, depending on the job state
		 @param {Object} job
		 @returns {String}
		 */
		utilities.prototype.getJobContextualColor = function(job) {
			if (job.statusshort === "idle") {
				return null;
			} else if (job.statusshort === "stopped") {
				return "warning";
			} else if (job.statusshort === "failed") {
				return "danger";
			} else if (job.statusshort === "complete") {
				return "success";
			} else {
				return "info";
			}
		};
		
		/**
		 Returns a jquery selector name, depending on the machine state
		 @param {Object} job
		 @returns {String}
		 */
		utilities.prototype.getMachineContextualColor = function(machine) {
			if (machine.statusshort === "idle") {
				return "";
			} else if (machine.statusshort === "busy") {
				return "success";
			} else if (machine.statusshort === "offline") {
				return "danger";
			} 
		};

		/**
		 Returns a hr string from milliseconds
		 @param {Number} milliseconds
		 @returns {String}
		 */
		utilities.prototype.msToStr = function(ms) {
			// days
			var outp = "";
			var s = Math.floor(ms / 1000);
			var days = Math.floor(( s = s % 31536000) / 86400);
			if (days) {
				outp += days + " " + (days === 1 ? t("IDS_WEB_DAY") : t("IDS_WEB_DAYS")) + " ";
			}
			// hous
			var hours = Math.floor(( s = s % 86400) / 3600);
			if (hours) {
				outp += hours + " " + (hours === 1 ? t("IDS_WEB_HOUR") : t("IDS_WEB_HOURS")) + " ";
			}
			// minutes
			var minutes = Math.floor(( s = s % 3600) / 60);
			if (minutes) {
				outp += minutes + " " + (minutes === 1 ? t("IDS_WEB_MINUTE") : t("IDS_WEB_MINUTES")) + " ";
			}
			// seconds
			var seconds = s % 60;
			if (seconds) {
				outp += seconds + " " + (seconds === 1 ? t("IDS_WEB_SECOND") : t("IDS_WEB_SECONDS")) + " ";
			}
			// zero?
			return outp.length > 0 ? outp : t("IDS_WEB_NOW");
		};

		/**
		 Formats a byte-dimension string.
		 @param {String} mem
		 @returns {String}
		 */
		utilities.prototype.memoryToString = function(bytes) {
		    var kb = 1024;
		    var mb = kb * 1024;
		    var gb = mb * 1024;
		    var tb = gb * 1024;
		   
		    if (bytes >= 0 && bytes < kb)
		        return bytes + ' B';
		    else if (bytes >= kb && bytes < mb)
		        return (bytes / kb).toFixed(2) + ' KB';
		    else if (bytes >= mb && bytes < gb)
		        return (bytes / mb).toFixed(2) + ' MB';
		    else if (bytes >= gb && bytes < tb)
		        return (bytes / gb).toFixed(2) + ' GB';
		    else
		        return (bytes / tb).toFixed(2) + ' TB';
		};

		/**
		 Capitalizes the first character.
		 @param {String} str
		 @returns {String}
		 */
		utilities.prototype.capitalizeFirstChar = function(str) {
			return str.charAt(0).toUpperCase() + str.slice(1);
		};

		/**
		 Returns a trimmed, lowercase string.
		 @param {String} value
		 @returns {String}
		 */
		utilities.prototype.cutdown = function(value) {
			return $.trim(value.toLowerCase());
		};

		/**
		 Strips HTML.
		 @param {String} value
		 @returns {String}
		 */
		utilities.prototype.HTML2Text = function(value) {
			return $("<div></div>").text(value).text();
		};

		/**
		 Tests if the given responsive breakpoint is active
		 @param {String} breakpoint - ("xs", "sm", "md" or "lg")
		 @return {Boolean} true if active
		 */
		utilities.prototype.testMediaBreakPoint = function(breakpoint) {
			return $(".device-" + breakpoint + ":visible").length != 0;
		};

		return utilities;
	})();

// entry point
$(document).ready(function() {
	// prepare utilities
	new utilities();
	// launch app
	new application();
});

// CINEMA 4D enums
enums = {
	DTYPE_NONE : 0,
	DTYPE_CHILDREN : 0,
	DTYPE_GROUP : 1,
	DTYPE_COLOR : 3,
	DTYPE_SUBCONTAINER : 5,
	DTYPE_MULTIPLEDATA : 6,
	DTYPE_TEXTURE : 7,
	DTYPE_BUTTON : 8,
	DTYPE_DYNAMIC : 10,
	DTYPE_SEPARATOR : 11,
	DTYPE_STATICTEXT : 12,
	DTYPE_POPUP : 13,
	DTYPE_LONG : 15,
	DTYPE_REAL : 19,
	DTYPE_TIME : 22,
	DTYPE_VECTOR : 23,
	DTYPE_MATRIX : 25,
	DTYPE_STRING : 130,
	DTYPE_FILENAME : 131,
	DTYPE_BASELISTLINK : 133,
	DTYPE_BOOL : 400006001,
	DTYPE_NORMAL : 400006005,
	DESCID_ROOT : 1000491,
	DETAILSELECTOR_NONE : 0,
	DETAILSELECTOR_SELECTED : 1,
	DETAILSELECTOR_NONSELECTED : 2,
	RDATA_SIZEUNIT_PIXELS : 0,
	RDATA_SIZEUNIT_CM : 1,
	RDATA_SIZEUNIT_MM : 2,
	RDATA_SIZEUNIT_INCHES : 3,
	RDATA_SIZEUNIT_POINTS : 4,
	RDATA_SIZEUNIT_PICAS : 5,
	DISABLE : 0,
	ENABLE : 1
};
