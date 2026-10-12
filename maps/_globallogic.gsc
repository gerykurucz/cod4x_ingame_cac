spawnPlayer()
{
	prof_begin( "spawnPlayer_preUTS" );

	self endon("disconnect");
	self endon("joined_spectators");
	self notify("spawned");
	self notify("end_respawn");

	self setSpawnVariables();

	if ( level.teamBased )
		self.sessionteam = self.team;
	else
		self.sessionteam = "none";

	hadSpawned = self.hasSpawned;

	self.sessionstate = "playing";
	self.spectatorclient = -1;
	self.killcamentity = -1;
	self.archivetime = 0;
	self.psoffsettime = 0;
	self.statusicon = "";
	if ( level.hardcoreMode )
		self.maxhealth = 30;
	else if ( level.oldschool )
		self.maxhealth = 200;
	else
		self.maxhealth = 100;
	self.health = self.maxhealth;
	
	self.friendlydamage = undefined;
	self.hasSpawned = true;
	self.spawnTime = getTime();
	self.afk = false;
	if ( self.pers["lives"] )
		self.pers["lives"]--;
	self.lastStand = undefined;

	if ( !self.wasAliveAtMatchStart )
	{
		acceptablePassedTime = 20;
		if ( level.timeLimit > 0 && acceptablePassedTime < level.timeLimit * 60 / 4 )
			acceptablePassedTime = level.timeLimit * 60 / 4;
		
		if ( level.inGracePeriod || getTimePassed() < acceptablePassedTime * 1000 )
			self.wasAliveAtMatchStart = true;
	}
	
	[[level.onSpawnPlayer]]();
	self maps\mp\gametypes\_missions::playerSpawned();

	prof_end( "spawnPlayer_preUTS" );

	level thread updateTeamStatus();
	
	prof_begin( "spawnPlayer_postUTS" );

	if ( level.oldschool )
	{
		assert( !isDefined( self.class ) );
		self maps\mp\gametypes\_oldschool::giveLoadout();
		self maps\mp\gametypes\_class::setClass( level.defaultClass );
	}
	else
	{
		if ( isDefined( self.pers["loadout_type"] ) && self.pers["loadout_type"] == "custom" )
		{
			self.pers["primaryWeapon"] = self.pers["queued_primary"] + "_mp";

			self maps\mp\gametypes\_teams::playerModelForWeapon( self.pers["queued_primary"] );

			self.pers["cac_primary"]   = self.pers["queued_primary"];
			self.pers["cac_p_attach"]  = self.pers["queued_p_attach"];
			self.pers["cac_camo"]      = self.pers["queued_camo"];
			self.pers["cac_secondary"] = self.pers["queued_secondary"];
			self.pers["cac_s_attach"]  = self.pers["queued_s_attach"];
			self.pers["cac_grenade"]   = self.pers["queued_grenade"];
			
			self.pers["cac_specialty1"] = "none";
            self.pers["cac_specialty2"] = "none";
            self.pers["cac_specialty3"] = "none";
			
			self giveCustomLoadout();
			
			if( !isDefined( self.class_num ) )
				self.class_num = 0;
		}

        else
        {
            assert( isValidClass( self.class ) );
            self maps\mp\gametypes\_class::setClass( self.class );
            self maps\mp\gametypes\_class::giveLoadout( self.team, self.class );
        }
	}

	if ( level.inPrematchPeriod )
	{
		self freezeControls( true );
		//self disableWeapons();
		
		self setClientDvar( "scr_objectiveText", getObjectiveHintText( self.pers["team"] ) );			

		team = self.pers["team"];
	}
	else
	{
		self freezeControls( false );
		self enableWeapons();
	}

	prof_end( "spawnPlayer_postUTS" );
	
	waittillframeend;
	self notify( "spawned_player" );
	self logstring( "S " + self.origin[0] + " " + self.origin[1] + " " + self.origin[2] );
	
	
	if ( game["state"] == "postgame" )
	{
		assert( !level.intermission );
		// We're in the victory screen, but before intermission
		self freezePlayerForRoundEnd();
	}
	self clearPerks();
}

giveCustomLoadout()
{
    self takeAllWeapons();
    p_weap = self.pers["cac_primary"];
    p_att  = self.pers["cac_p_attach"];
    
    if ( !isDefined( p_weap ) ) p_weap = "ak47";
    
    p_full = p_weap;
    if ( isDefined( p_att ) && p_att != "none" && p_att != "" )
        p_full = p_weap + "_" + p_att;
    
    p_full = p_full + "_mp";

    c_idx = 0;
    c_name = self.pers["cac_camo"];
    
    if ( isDefined( c_name ) )
    {
        c_name = toLower(c_name);

        if ( isSubStr( c_name, "gold" ) )
        {
            if ( self getStat( 260 ) > 0 ) 
                c_idx = 6; 
            else 
                self iprintlnbold( "Gold camo is not unlocked yet." );
        }
        else if ( isSubStr( c_name, "red_tiger" ) )   c_idx = 4;
        else if ( isSubStr( c_name, "blue_tiger" ) )  c_idx = 5;
        else if ( isSubStr( c_name, "digital" ) )     c_idx = 3;
        else if ( isSubStr( c_name, "woodland" ) )    c_idx = 2;
        else if ( isSubStr( c_name, "desert" ) )      c_idx = 1;
    }
	
    s_weap = self.pers["cac_secondary"];
    s_att  = self.pers["cac_s_attach"];
    
    if ( !isDefined( s_weap ) ) s_weap = "beretta";
    
    s_full = s_weap;
    if ( isDefined( s_att ) && s_att != "none" && s_att != "" )
        s_full = s_weap + "_" + s_att;
        
    s_full = s_full + "_mp";

    s_gren_ref = self.pers["cac_grenade"];
    if ( !isDefined( s_gren_ref ) || s_gren_ref == "" ) 
        s_gren_ref = "flash_grenade";
    
    s_gren_full = s_gren_ref + "_mp";

    self giveWeapon( s_full );
    self giveMaxAmmo( s_full );

    self giveWeapon( p_full, int(c_idx) );
    self giveMaxAmmo( p_full );
	self setSpawnWeapon( p_full );

    self giveWeapon( "frag_grenade_mp" );
    self setWeaponAmmoClip( "frag_grenade_mp", 1 );


    if ( isSubStr( s_gren_ref, "flash" ) )
        self setOffhandSecondaryClass( "flash" );
    else
        self setOffhandSecondaryClass( "smoke" );

    self giveWeapon( s_gren_full );
    self setWeaponAmmoClip( s_gren_full, 1 );
}