init()
{
	game["menu_team"] = "team_marinesopfor";
	game["menu_class_allies"] = "class_marines";
	game["menu_changeclass_allies"] = "changeclass_marines";
	game["menu_initteam_allies"] = "initteam_marines";
	game["menu_class_axis"] = "class_opfor";
	game["menu_changeclass_axis"] = "changeclass_opfor";
	game["menu_initteam_axis"] = "initteam_opfor";
	game["menu_class"] = "class";
	game["menu_changeclass"] = "changeclass";
	game["menu_changeclass_offline"] = "changeclass_offline";

	if ( !level.console )
	{
		game["menu_callvote"] = "callvote";
		game["menu_muteplayer"] = "muteplayer";
		precacheMenu(game["menu_callvote"]);
		precacheMenu(game["menu_muteplayer"]);
		
		// ---- back up one folder to access game_summary.menu ----
		// game summary menu file precache
		game["menu_eog_main"] = "endofgame";
		
		// menu names (do not precache since they are in game_summary_ingame which should be precached
		game["menu_eog_unlock"] = "popup_unlock";
		game["menu_eog_summary"] = "popup_summary";
		game["menu_eog_unlock_page1"] = "popup_unlock_page1";
		game["menu_eog_unlock_page2"] = "popup_unlock_page2";
		
		precacheMenu(game["menu_eog_main"]);
		precacheMenu(game["menu_eog_unlock"]);
		precacheMenu(game["menu_eog_summary"]);
		precacheMenu(game["menu_eog_unlock_page1"]);
		precacheMenu(game["menu_eog_unlock_page2"]);
	
	}
	else
	{
		game["menu_controls"] = "ingame_controls";
		game["menu_options"] = "ingame_options";
		game["menu_leavegame"] = "popup_leavegame";

		if(level.splitscreen)
		{
			game["menu_team"] += "_splitscreen";
			game["menu_class_allies"] += "_splitscreen";
			game["menu_changeclass_allies"] += "_splitscreen";
			game["menu_class_axis"] += "_splitscreen";
			game["menu_changeclass_axis"] += "_splitscreen";
			game["menu_class"] += "_splitscreen";
			game["menu_changeclass"] += "_splitscreen";
			game["menu_controls"] += "_splitscreen";
			game["menu_options"] += "_splitscreen";
			game["menu_leavegame"] += "_splitscreen";
		}

		precacheMenu(game["menu_controls"]);
		precacheMenu(game["menu_options"]);
		precacheMenu(game["menu_leavegame"]);
	}

	precacheMenu("scoreboard");
	precacheMenu(game["menu_team"]);
	precacheMenu(game["menu_class_allies"]);
	precacheMenu(game["menu_changeclass_allies"]);
	precacheMenu(game["menu_initteam_allies"]);
	precacheMenu(game["menu_class_axis"]);
	precacheMenu(game["menu_changeclass_axis"]);
	precacheMenu(game["menu_class"]);
	precacheMenu(game["menu_changeclass"]);
	precacheMenu(game["menu_initteam_axis"]);
	precacheMenu(game["menu_changeclass_offline"]);
	precacheString( &"MP_HOST_ENDED_GAME" );
	precacheString( &"MP_HOST_ENDGAME_RESPONSE" );

	level thread onPlayerConnect();
}

onPlayerConnect()
{
	for(;;)
	{
		level waittill("connecting", player);

		player setClientDvar("ui_3dwaypointtext", "1");
		player.enable3DWaypoints = true;
		player setClientDvar("ui_deathicontext", "1");
		player.enableDeathIcons = true;
		self.pers["loadout_type"] = "default";	
		player thread onMenuResponse();
	}
}

onMenuResponse()
{
    self endon("disconnect");
    
    for(;;)
    {
        self waittill("menuresponse", menu, response);
		
		if ( isSubStr( response, "_mp" ) && !isSubStr( response, "loadout_" ) )
        {
            self.pers["loadout_type"] = "default";
        }
        
        tokens = strTok( response, ":" );
        
        if ( tokens.size >= 2 )
        {
            type = tokens[0];
            val  = tokens[1];

            if ( type == "loadout_primary" )              self.pers["cac_primary"] = val;
            if ( type == "loadout_primary_attachment" )   self.pers["cac_p_attach"] = val;
            if ( type == "loadout_secondary" )            self.pers["cac_secondary"] = val;
            if ( type == "loadout_secondary_attachment" ) self.pers["cac_s_attach"] = val;
            if ( type == "loadout_camo" )                 self.pers["cac_camo"] = val;
            if ( type == "loadout_grenade" )              self.pers["cac_grenade"] = val;
            
            continue;
        }

        if( response == "takeloadout" )
        {
            self maps\mp\gametypes\_class::cac_getdata();

            if ( !isDefined( self.pers["cac_primary"] ) )
            {
                full_primary = self.custom_class[4]["primary"];
                p_tokens = strTok( full_primary, "_" );
                self.pers["cac_primary"] = p_tokens[0];
                
                if ( !isDefined( self.pers["cac_p_attach"] ) )
                {
                    if ( p_tokens.size > 2 ) self.pers["cac_p_attach"] = p_tokens[1];
                    else                     self.pers["cac_p_attach"] = "none";
                }
            }
            else if ( !isDefined( self.pers["cac_p_attach"] ) ) 
            {
                self.pers["cac_p_attach"] = "none";
            }

            if ( !isDefined( self.pers["cac_secondary"] ) )
            {
                full_secondary = self.custom_class[4]["secondary"];
                s_tokens = strTok( full_secondary, "_" );
                secondary_base = s_tokens[0];
                
                self.pers["cac_secondary"] = secondary_base;

                if ( secondary_base == "deserteagle" || secondary_base == "deserteaglegold" )
                {
                    self.pers["cac_s_attach"] = "none";
                }
                else if ( !isDefined( self.pers["cac_s_attach"] ) )
                {
                    if ( s_tokens.size > 2 ) 
                        self.pers["cac_s_attach"] = s_tokens[1]; 
                    else 
                        self.pers["cac_s_attach"] = "none";
                }
            }
            else 
            {
                if ( self.pers["cac_secondary"] == "deserteagle" || self.pers["cac_secondary"] == "deserteaglegold" )
                {
                    self.pers["cac_s_attach"] = "none";
                }
                else if ( !isDefined( self.pers["cac_s_attach"] ) )
                {
                    self.pers["cac_s_attach"] = "none";
                }
            }

            if ( !isDefined( self.pers["cac_camo"] ) )
            {
                c_idx = self.custom_class[4]["camo_num"];
                if ( c_idx == 6 )      self.pers["cac_camo"] = "gold";
                else if ( c_idx == 5 ) self.pers["cac_camo"] = "blue_tiger";
                else if ( c_idx == 4 ) self.pers["cac_camo"] = "red_tiger";
                else if ( c_idx == 3 ) self.pers["cac_camo"] = "digital";
                else if ( c_idx == 2 ) self.pers["cac_camo"] = "woodland";
                else if ( c_idx == 1 ) self.pers["cac_camo"] = "desert";
                else                   self.pers["cac_camo"] = "none";
            }

            if ( !isDefined( self.pers["cac_grenade"] ) )
            {
                full_grenade = self.custom_class[4]["special_grenade"];
                g_tokens = strTok( full_grenade, "_" );
                self.pers["cac_grenade"] = g_tokens[0] + "_" + g_tokens[1]; 
            }
            self.pers["loadout_type"] = "custom";
            self.class = "heavygunner_mp"; 
            self.pers["class"] = "heavygunner_mp";
            self.pers["queued_primary"]   = self.pers["cac_primary"];
            self.pers["queued_p_attach"]  = self.pers["cac_p_attach"];
            self.pers["queued_camo"]      = self.pers["cac_camo"];
            self.pers["queued_secondary"] = self.pers["cac_secondary"];
            self.pers["queued_s_attach"]  = self.pers["cac_s_attach"];
            self.pers["queued_grenade"]   = self.pers["cac_grenade"];
            
            if( self.sessionstate == "spectator" )
            {
                if ( level.gametype == "sd" )
                {
                    canSpawn = false;
                    
                    if ( !isDefined( self.pers["lives"] ) )
                    {
                        canSpawn = true;
                    }

                    else if ( self.pers["lives"] > 0 && (isDefined(level.inGracePeriod) && level.inGracePeriod) )
                    {
                        if( !isDefined(self.hasSpawned) || !self.hasSpawned )
                            canSpawn = true;
                    }

                    if ( canSpawn )
                    {
                        if ( !isDefined( self.pers["lives"] ) || self.pers["lives"] == 0 )
                            self.pers["lives"] = 1;

                        self.sessionteam = self.pers["team"];
                        self thread spawn_after_selection();
                    }
                    else 
                    {
                        self iprintlnbold( "Your class will change the next time you spawn!" );
                    }
                }
                else
                {
                    self thread spawn_after_selection();
                }
            }
			
			else if( isDefined(level.inGracePeriod) && level.inGracePeriod )
			{
				self nektum\nektum_ecosystem::loadoutQueue();
				self closeMenu();
				self closeInGameMenu();
				continue;
			}
			
            else if( self.sessionstate == "playing" || self.sessionstate == "death" )
            {
                self iprintlnbold( "Your class will change the next time you spawn!" );
            }
			
            self closeMenu();
            self closeInGameMenu();            
        }
		
		if ( response == "back" )
		{
			self closeMenu();
			self closeInGameMenu();

			if ( level.console )
			{
				if( menu == game["menu_changeclass"] || menu == game["menu_changeclass_offline"] || menu == game["menu_team"] || menu == game["menu_controls"] )
				{
					if( self.pers["team"] == "allies" )
						self openMenu( game["menu_class_allies"] );
					if( self.pers["team"] == "axis" )
						self openMenu( game["menu_class_axis"] );
				}
			}
			continue;
		}
		
		if(response == "changeteam")
		{
			self closeMenu();
			self closeInGameMenu();
			self openMenu(game["menu_team"]);
		}
	
		if(response == "changeclass_marines" )
		{
			self closeMenu();
			self closeInGameMenu();
			self openMenu( game["menu_changeclass_allies"] );
		}

		if(response == "changeclass_opfor" )
		{
			self closeMenu();
			self closeInGameMenu();
			self openMenu( game["menu_changeclass_axis"] );
		}

		if(response == "changeclass_marines_splitscreen" )
			self openMenu( "changeclass_marines_splitscreen" );

		if(response == "changeclass_opfor_splitscreen" )
			self openMenu( "changeclass_opfor_splitscreen" );
					
		// rank update text options
		if(response == "xpTextToggle")
		{
			self setClientDvar( "ui_xpText", "0" );
			continue;
		}

		// 3D Waypoint options
		if(response == "waypointToggle")
		{
			self.enable3DWaypoints = !self.enable3DWaypoints;
			if (self.enable3DWaypoints)
				self setClientDvar( "ui_3dwaypointtext", "1" );
			else
				self setClientDvar( "ui_3dwaypointtext", "0" );
			continue;
		}

		// 3D death icon options
		if(response == "deathIconToggle")
		{
			self setClientDvar( "ui_deathicontext", "0" );
			continue;
		}
		
		if(response == "endgame")
		{
			// TODO: replace with onSomethingEvent call 
			if(level.splitscreen)
			{
				if ( level.console )
					endparty();
				level.skipVote = true;

				if ( !level.gameEnded )
				{
					level thread maps\mp\gametypes\_globallogic::forceEnd();
				}
			}
				
			continue;
		}

		if ( response == "endround" && level.console )
		{
			if ( !level.gameEnded )
			{
				level thread maps\mp\gametypes\_globallogic::forceEnd();
			}
			else
			{
				self closeMenu();
				self closeInGameMenu();
				self iprintln( &"MP_HOST_ENDGAME_RESPONSE" );
			}			
			continue;
		}

		if(menu == game["menu_team"])
		{
			switch(response)
			{
			case "allies":
				//self closeMenu();
				//self closeInGameMenu();
				self [[level.allies]]();
				break;

			case "axis":
				//self closeMenu();
				//self closeInGameMenu();
				self [[level.axis]]();
				break;

			case "autoassign":
				//self closeMenu();
				//self closeInGameMenu();
				self [[level.autoassign]]();
				break;

			case "spectator":
				//self closeMenu();
				//self closeInGameMenu();
				self [[level.spectator]]();
				break;
			}
		}	// the only responses remain are change class events
		else if( menu == game["menu_changeclass"] || menu == game["menu_changeclass_offline"] )
		{
			self closeMenu();
			self closeInGameMenu();

			self.selectedClass = true;
			self [[level.class]](response);
		}
		else if ( !level.console )
		{
			if(menu == game["menu_quickcommands"])
				maps\mp\gametypes\_quickmessages::quickcommands(response);
			else if(menu == game["menu_quickstatements"])
				maps\mp\gametypes\_quickmessages::quickstatements(response);
			else if(menu == game["menu_quickresponses"])
				maps\mp\gametypes\_quickmessages::quickresponses(response);
		}
		
	}
}

spawn_after_selection()
{
    self endon("disconnect");
    
    wait 0.1;
    
    if( self.pers["team"] == "allies" || self.pers["team"] == "axis" )
    {
        self maps\mp\gametypes\_globallogic::spawnPlayer();
    }
    else 
    {
	
    }
}