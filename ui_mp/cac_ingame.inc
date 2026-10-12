// general definitions

#define CHOICE_X_START			0
#define CHOICE_Y_START			34

#define CHOICE_SEP_1			3
#define CHOICE_SEP_2			6

#include "ui_mp/menustyle.inc"
//#undef CHOICE_SIZE_X
//#define CHOICE_SIZE_X			216

#include "ui/choices_setup_common.menu"
 
#define ORIGIN_STATUS			390 20
#define MENU_FONT_COLOR2		1 1 1 0.75

#include "ui_mp/stats_info.inc"

	menuDef	{
		name			CAC_MENU_NAME
		//fullscreen		1
		rect			0 0 640 480
		focuscolor		COLOR_FOCUSED
		style			WINDOW_STYLE_EMPTY //style			WINDOW_STYLE_FILLED
		blurWorld		7.0
		onOpen {			
			hidemenu "endofgame";
			// closing the menus behind due to fullscreen 1, which allows buttons behind the menu to be clicked outside the 4:3 area
			close "pc_cac_popup_ingame";
			close "pc_cac_popup";
			
			// load perk 2's selection into ui_perk2_selection so we can check if dual primaries perk is selected
			execnow "statGetInDvar "CAC_S_SPECIALTY_WEAPON" ui_perk2_selection";
			
			execnow "set ui_dual_primaries false";
			execondvarintvalue ui_perk2_selection 166 "set ui_dual_primaries true";
					
			// set focus to first menu selection	
			setfocus primary_weapon_selection;		
		}
		onEsc {
			execnow "uploadstats";
			close CAC_MENU_NAME;
		}
		onClose{
			hidemenu "endofgame";
		}

		// ------------------  statics ------------------------
		itemDef
		{
			style			WINDOW_STYLE_SHADER
			rect			FULLSCREEN_WIDE
			background		"animbg_blur_back"
			visible			when( !dvarbool( ui_showEndOfGame ) )
			decoration
		}

		#undef FOG_SCROLL_TIME
		#define FOG_SCROLL_TIME 60000 //30000

		itemDef
		{
			style			WINDOW_STYLE_SHADER
			rect			-107 0 1708 480
			exp				rect X((-107) - ((float(milliseconds() % FOG_SCROLL_TIME) / FOG_SCROLL_TIME) * (854)))
			background		"animbg_blur_fogscroll"
			visible			when( !dvarbool( ui_showEndOfGame ) )
			decoration
		}

		itemDef
		{
			style			WINDOW_STYLE_SHADER
			rect			FULLSCREEN_WIDE
			background		"animbg_blur_front"
			visible			when( !dvarbool( ui_showEndOfGame ) )
			decoration
		}
		
		// ----------------------------------------------------------------
		
		// background overlay
		itemDef
		{
			style			WINDOW_STYLE_FILLED
			rect			0 0 640 480 HORIZONTAL_ALIGN_FULLSCREEN VERTICAL_ALIGN_FULLSCREEN
			backcolor		0 0 0 0.75 //0 0.05 0.15 0.5
			visible			when( dvarbool( ui_showEndOfGame ) )
			decoration
		}

		// gradient_top
		itemDef
		{
			style			WINDOW_STYLE_SHADER
			rect			0 0 854 75 HORIZONTAL_ALIGN_FULLSCREEN 0
			background		"gradient_top"
			visible			when( dvarbool( ui_showEndOfGame ) )
			decoration
		}

		// gradient_bottom
		itemDef
		{
			style			WINDOW_STYLE_SHADER
			rect			0 405 854 75 HORIZONTAL_ALIGN_FULLSCREEN 0
			background		"gradient_bottom"
			visible			when( dvarbool( ui_showEndOfGame ) )
			decoration
		}
		

		CHOICE_MENU_TITLE( "INVENTORY" )				
		// ------------------------- buttons ------------------------------
		#define BACK_OPEN ;
		#include "ui_mp/navcontrols.inc"
		
		// ------------------------ lobby status ----------------------------
		itemDef
		{
			type			ITEM_TYPE_TEXT
			visible			1
			rect			ORIGIN_STATUS 0 0
			origin			0 0
			forecolor		MENU_FONT_COLOR2
			text			"@MENU_MATCH_BEGINNING_IN"
			textfont		UI_FONT_NORMAL
			textscale		TEXTSIZE_SMALL
			visible 		when ( dvarBool( ui_showEndOfGame ) );
			decoration
		}
		itemDef
		{
			name			intermission_timer_countdown
			type			ITEM_TYPE_TEXT
			visible			1
			rect			ORIGIN_STATUS 0 0
			origin			-40 0
			forecolor		MENU_FONT_COLOR2
			exp				text( secondsAsCountdown( timeLeft() ) );
			textfont		UI_FONT_NORMAL
			textscale		TEXTSIZE_SMALL
			visible 		when ( dvarBool( ui_showEndOfGame ) );
			decoration
		}
		
		//=========================================================
		//===================== MENU SELECTION ====================
		//=========================================================
		
		// PRIMARY WEAPON BUTTON =============================================================
		#define CAC_SETUP_ACTION1 \
			execnow "set ui_primary_selected Primary Weapon"; \
			play "mouse_click"; \
			open CAC_PREFIX"popup_cac_primary";
		
		CHOICE_BUTTON_EX( 1, "@MPUI_PRIMARY_WEAPON1", CAC_SETUP_ACTION1, name primary_weapon_selection )
		CHOICE_NEWICON_VIS( 1, "specialty_new", when( statRangeAnyBitsSet( 3010, 3089, WEAPON_NEW_BITMASK ) ); )

		// SECONDARY WEAPON/SIDE ARM BUTTON =============================================================
		#define CAC_SETUP_ACTION2 \
			play "mouse_click"; \
			open CAC_PREFIX"popup_cac_secondary";

		#define CAC_SETUP_ACTION2_2 \
			execnow "set ui_primary_selected Secondary Weapon"; \
			play "mouse_click"; \
			open CAC_PREFIX"popup_cac_primary2";
					
		CHOICE_BUTTON_BG( 2, 1 )
		CHOICE_HIGHLIGHT( 2, 1 )
		CHOICE_BUTTON_VIS_NOHI( 2, "@MPUI_SIDE_ARM1", CAC_SETUP_ACTION2, when( "specialty_twoprimaries" != tablelookup( "mp/statstable.csv", 1, stat(CAC_SPECIALTY_WEAPON) , 4 ) ); )
		CHOICE_NEWICON_VIS( 2, "specialty_new", when( statRangeAnyBitsSet( 3000, 3009, WEAPON_NEW_BITMASK ) && "specialty_twoprimaries" != tablelookup( "mp/statstable.csv", 1, stat(CAC_SPECIALTY_WEAPON) , 4  ) ); )
		CHOICE_BUTTON_VIS_NOHI( 2, "@MPUI_SECONDARY_WEAPON", CAC_SETUP_ACTION2_2, when( "specialty_twoprimaries" == tablelookup( "mp/statstable.csv", 1, stat(CAC_SPECIALTY_WEAPON) , 4  ) ); )
	
		// SPECIAL GRENADE BUTTON =============================================================
		#define CAC_SETUP_ACTION3 \
			play "mouse_click"; \
			open CAC_PREFIX"popup_cac_extra";
			
		CHOICE_BUTTON( 3, "@MPUI_SPECIAL_GRENADE", CAC_SETUP_ACTION3 )

		CHOICE_SEPARATOR( CHOICE_SEP_1 )
		

		// TAKE LOADOUT BUTTON =============================================================	
		#define CAC_SETUP_ACTION7 \		
			play "mouse_click"; \
			scriptMenuResponse "takeloadout";
		
		#ifdef PC
			CHOICE_BUTTON( 4, "Take Loadout!", CAC_SETUP_ACTION7 )
		#else
			CHOICE_BUTTON( 4, "Take Loadout!", CAC_SETUP_ACTION7 )
		#endif
				
		/* ================================================================================= */
		/* ================================ LOADOUT DISPLAY ================================ */
		/* ================================================================================= */

		#define STAT_CAC_PRIMARY				stat( CAC_PRIMARY )
		#define STAT_CAC_PRIMARY_ATTACHMENT		stat( CAC_PRIMARY_ATTACHMENT )
		#define STAT_CAC_SECONDARY				stat( CAC_SECONDARY )
		#define STAT_CAC_SECONDARY_ATTACHMENT	stat( CAC_SECONDARY_ATTACHMENT )
		#define STAT_CAC_SPECIALTY_EQUIPMENT	stat( CAC_SPECIALTY_EQUIPMENT )
		#define STAT_CAC_SPECIALTY_WEAPON		stat( CAC_SPECIALTY_WEAPON )
		#define STAT_CAC_SPECIALTY_ABILITY		stat( CAC_SPECIALTY_ABILITY )
		#define STAT_CAC_SPECIAL_GRENADE		stat( CAC_SPECIAL_GRENADE )
		#define STAT_CAC_CAMO					stat( CAC_CAMO )
		
		#include "ui_mp/cac_loadout_ingame.inc"

		#include "ui/safearea.menu"
	}
	
	// close all 2nd and 3rd layer popups on end action
	#define PREPROC_ATTACH_CLOSEALL \
	close CAC_PREFIX"popup_cac_primary"; \
	close CAC_PREFIX"popup_cac_primary2"; \
	close CAC_PREFIX"popup_cac_secondary"; \
	close CAC_PREFIX"popup_cac_assault"; \
	close CAC_PREFIX"popup_cac_SMG"; \
	close CAC_PREFIX"popup_cac_LMG"; \
	close CAC_PREFIX"popup_cac_sniper"; \
	close CAC_PREFIX"popup_cac_shotgun"; \
	close CAC_PREFIX"popup_cac_assault2"; \
	close CAC_PREFIX"popup_cac_SMG2"; \
	close CAC_PREFIX"popup_cac_LMG2"; \
	close CAC_PREFIX"popup_cac_sniper2"; \
	close CAC_PREFIX"popup_cac_shotgun2"; \
	close "popup_cac_template"; \
	close CAC_PREFIX"attachment_popup_assault"; \
	close CAC_PREFIX"attachment_popup_SMG"; \
	close CAC_PREFIX"attachment_popup_LMG"; \
	close CAC_PREFIX"attachment_popup_sniper"; \
	close CAC_PREFIX"attachment_popup_shotgun"; \
	close CAC_PREFIX"attachment_popup_pistol"; \
	close CAC_PREFIX"attachment_popup_assault2"; \
	close CAC_PREFIX"attachment_popup_SMG2"; \
	close CAC_PREFIX"attachment_popup_LMG2"; \
	close CAC_PREFIX"attachment_popup_sniper2"; \
	close CAC_PREFIX"attachment_popup_shotgun2"; \
	close CAC_PREFIX"attachment_popup_pistol2"; \
	close CAC_PREFIX"popup_cac_camo"; \
	close CAC_PREFIX"popup_cac_camo2"; \
	close CAC_PREFIX"attachment_popup_fake";
	
	
	// including weapon data
	#include "ui_mp/weaponinfo.menu"
	
#include "ui_mp/popupstyle.inc"	
#include "ui/choices_setup_popmenu.menu"

#undef CHOICE_SIZE_X
#define CHOICE_SIZE_X			216

#undef NEW_X_OFFSET			
#define NEW_X_OFFSET	(0-CHOICE_SIZE_X)

#undef NEW_Y_OFFSET			
#define NEW_Y_OFFSET	(0-2)

	// ========================== Perk 1 special cases ===========================
	
	#define PERK1_RESET1 \
		execNowOnDvarStringValue ui_perk1_slot 193 "statset "CAC_S_SPECIALTY_EQUIPMENT" 190"; \
		execNowOnDvarStringValue ui_perk1_slot 192 "statset "CAC_S_SPECIALTY_EQUIPMENT" 190"; \
		execNowOnDvarStringValue ui_perk1_slot 191 "statset "CAC_S_SPECIALTY_EQUIPMENT" 190"; 
		
	#define PERK1_RESET \
		execNow "statGetInDvar "CAC_S_SPECIALTY_EQUIPMENT" ui_perk1_slot"; PERK1_RESET1 \
		execNow "statGetInDvar "CAC_S_SECONDARY_ATTACHMENT" ui_secondary_attachment_slot"; \
		execNowOnDvarStringValue ui_secondary_attachment_slot 4 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193"; \
		execNowOnDvarStringValue ui_secondary_attachment_slot 5 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193"; \
		execNow "statGetInDvar "CAC_S_PRIMARY_ATTACHMENT" ui_primary_attachment_slot"; \
		execNowOnDvarStringValue ui_primary_attachment_slot 4 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193"; \
		execNowOnDvarStringValue ui_primary_attachment_slot 5 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193";
		
	// ====================================================================================================
	// primary weapon selection ===========================================================================
	// ====================================================================================================
	#define LOCAL_WEAPON_INFO_WINDOW( highlight_dvar ) \
		/* weapon information side frame*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) -4 280 CHOICE_POPUP_HEIGHT( 7 ) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 1, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" && dvarString(ui_inside_popup) != "camo" ); ) \
		LOADOUT_PLATING_RAW( CHOICE_POPUP_WIDTH 8, -2, 0, 280, (CHOICE_POPUP_HEIGHT( 7 )-4), CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN visible when( dvarString(ui_inside_popup) != "attachment" && dvarString(ui_inside_popup) != "camo" );, 0, 0 )\
		/* weapon image*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 30 256 48 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "gradient_bottom", 1 1 1 0.3, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" && dvarString(ui_inside_popup) != "camo" ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+72) -6 180 90 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),6), 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" && dvarString(ui_inside_popup) != "camo" ); ) \
		/* weapon title*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 58 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),3), TEXTSIZE_DEFAULT, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( dvarString(ui_inside_popup) != "attachment" && dvarString(ui_inside_popup) != "camo" ); ) \
		/* weapon desc unlocked*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),7), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, CHOICE_TEXTCOLOR visible when( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ); )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),10), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( ( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ) == 0 ); )\
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 0 (CHOICE_SIZE_Y*2) CHOICE_SIZE_Y CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "specialty_locked", 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( ( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ) == 0 ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( 0 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", 0.15 0.15 0.17 1, 0, 2, CHOICE_POPUP_BORDER_COLOR )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( 8 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, -32 0, dvarString(ui_weapon_class_selected), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_RIGHT, CHOICE_TEXTCOLOR ) \
		/* pointer icon */ \
		PREPROC_SHADER_DRAW_ALIGNED( 200 -16 16 8 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "hitech_arrow_down", 0.9 0.9 0.95 0.4, 0, 2, CHOICE_POPUP_BORDER_COLOR )

	#define LOCAL_PRIMARY_WEAPON_ACTION( suffix, pstat, weapRef ) \
		execnow "statset "CAC_S_PRIMARY_ATTACHMENT" 0; statset "CAC_S_CAMO" 0"; \
		play "mouse_click"; \
		scriptMenuResponse "loadout_primary:"weapRef;\
		statsetusingtable( CAC_PRIMARY, pstat ); \
		statsetusingtable( CAC_PRIMARY_STORAGE, tableLookup( "mp/statstable.csv", 4, weapRef, 1 ) ); \
		execNow "statGetInDvar "CAC_S_PRIMARY_STORAGE" ui_primary_weapon"; \
		CLEAR_NEW( ui_primary_weapon, WEAPON_NEW_BITMASK ); \
		execNow "statGetInDvar "CAC_S_SPECIALTY_EQUIPMENT" ui_perk1_slot"; PERK1_RESET1 \
		execNow "statGetInDvar "CAC_S_SECONDARY_ATTACHMENT" ui_secondary_attachment_slot"; \
		execNowOnDvarStringValue ui_secondary_attachment_slot 4 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193"; \
		execNowOnDvarStringValue ui_secondary_attachment_slot 5 "statset "CAC_S_SPECIALTY_EQUIPMENT" 193"; \
		uiScript openMenuOnDvar "selected_weapon_class" assault CAC_PREFIX"attachment_popup_assault"suffix; \
		uiScript openMenuOnDvar "selected_weapon_class" SMG CAC_PREFIX"attachment_popup_SMG"suffix; \
		uiScript openMenuOnDvar "selected_weapon_class" LMG CAC_PREFIX"attachment_popup_LMG"suffix; \
		uiScript openMenuOnDvar "selected_weapon_class" shotgun CAC_PREFIX"attachment_popup_shotgun"suffix; \
		uiScript openMenuOnDvar "selected_weapon_class" sniper CAC_PREFIX"attachment_popup_sniper"suffix; \
		execnow "uploadstats";

		
	// hacked to hide the second primary if same weapon is selected in first primary, change weapon description to "Already selected"
	#define LOCAL_WEAPON_ITEM( suffix, itemNum, weaponName, weaponStat, weaponRef, highlight_dvar, rawStat )\
		CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "weaponRef, ;, when( ( stat( int(weaponStat)+3000 ) & 1 ) == 0 ); ) \
		CHOICE_DBUTTON_VIS( itemNum, weaponName, when( 1 ); ) \
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_PRIMARY_WEAPON_ACTION( suffix, weaponStat, weaponRef ), \
		execnow "set "highlight_dvar" "weaponRef;, CLEAR_WEAP_NEW( weaponRef );, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" && int(weaponStat) != stat(CAC_SECONDARY) );, \
		( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" && int(weaponStat) != stat(CAC_SECONDARY) ) \
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_PRIMARY_WEAPON_ACTION2( suffix, weaponStat, weaponRef ), \
		execnow "set "highlight_dvar" "weaponRef;, CLEAR_WEAP_NEW( weaponRef );, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Secondary Weapon" && int(weaponStat) != stat(CAC_PRIMARY) );, \
		( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Secondary Weapon" && int(weaponStat) != stat(CAC_PRIMARY) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( ( stat( int(weaponStat)+3000 ) & (NEW_WEAPON_BITMASK+NEW_ITEMS_MASK)) > 0 ) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_locked", when( ( stat( int(weaponStat)+3000 ) & 1 ) == 0 ); ) 
		
	#define LOCAL_WEAPON_ITEM2( suffix, itemNum, weaponName, weaponStat, weaponRef, highlight_dvar, rawStat )\
		CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "weaponRef, ;, when( ( stat( int(weaponStat)+3000 ) & 1 ) == 0 ); ) \
		CHOICE_DBUTTON_VIS( itemNum, weaponName, when( 1 ); ) \
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_PRIMARY_WEAPON_ACTION3( suffix, weaponStat, weaponRef ), \
		execnow "set "highlight_dvar" "weaponRef;, CLEAR_WEAP_NEW( weaponRef );, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" && int(weaponStat) != stat(CAC_SECONDARY) );, \
		( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" && int(weaponStat) != stat(CAC_SECONDARY) ) \
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_PRIMARY_WEAPON_ACTION4( suffix, weaponStat, weaponRef ), \
		execnow "set "highlight_dvar" "weaponRef;, CLEAR_WEAP_NEW( weaponRef );, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Secondary Weapon" && int(weaponStat) != stat(CAC_PRIMARY) );, \
		( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Secondary Weapon" && int(weaponStat) != stat(CAC_PRIMARY) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( ( stat( int(weaponStat)+3000 ) & (NEW_WEAPON_BITMASK+NEW_ITEMS_MASK)) > 0 ) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_locked", when( ( stat( int(weaponStat)+3000 ) & 1 ) == 0 ); ) 
		
	/*			
	#define LOCAL_WEAPON_ITEM_HIDDEN( suffix, itemNum, weaponName, weaponStat, weaponRef, highlight_dvar, rawStat )\
		CHOICE_BUTTON_BG( itemNum, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" ); ) \
		CHOICE_BUTTON_FOCUS_VIS( itemNum, weaponName, LOCAL_PRIMARY_WEAPON_ACTION3( suffix, weaponStat, weaponRef );, execnow "set "highlight_dvar" "weaponRef, CLEAR_WEAP_NEW( weaponRef );, when( ( stat( int(weaponStat)+3000 ) & 1 ) && dvarString( ui_primary_selected ) == "Primary Weapon" ); ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( (stat( int(weaponStat)+3000 ) & (NEW_WEAPON_BITMASK+NEW_ITEMS_MASK)) > 0 && dvarString( ui_primary_selected ) == "Primary Weapon" ); )	
	*/
	
	// primary weapon class dropdown
	menuDef
	{
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_primary", 4, (CHOICE_X( 1 )-2), (CHOICE_Y( 1 )-4), ;, execnow "set ui_inside_popup weapon_class; set ui_show_preview 1";, 1 )
		onClose{	execnow "set ui_show_preview 0; set ui_primary_highlighted 0; set ui_attachment_highlighted 0";	}
		
		#define LOCAL_WEAPON_CLASS( itemNum, ptext, plabel, pVis ) \
			CHOICE_BUTTON_EX( itemNum, plabel, play "mouse_click"; execnow "set selected_weapon_class "ptext"; set ui_weapon_class_selected "plabel; open CAC_PREFIX"popup_cac_"ptext;, name ptext ) \
			CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( pVis ) )		

		LOCAL_WEAPON_CLASS( 1, "assault", "@MPUI_ASSAULT_RIFLES", statRangeAnyBitsSet( 3020, 3026, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS( 2, "SMG", "@MPUI_SUB_MACHINE_GUNS", statRangeAnyBitsSet( 3010, 3019, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS( 3, "shotgun", "@MPUI_SHOTGUNS", statRangeAnyBitsSet( 3070, 3079, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS( 4, "sniper", "@MPUI_SNIPER_RIFLES", statRangeAnyBitsSet( 3060, 3069, WEAPON_NEW_BITMASK ) )	
	}
	
	// second primary weapon class dropdown
	menuDef
	{
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_primary2", 4, (CHOICE_X( 2 )-2), (CHOICE_Y( 2 )), ;, execnow "set ui_inside_popup weapon_class; set ui_show_preview 1";, 1 )
		onClose{	execnow "set ui_show_preview 0; set ui_primary_highlighted 0; set ui_attachment_highlighted 0";	}
		
		#define LOCAL_WEAPON_CLASS2( itemNum, ptext, plabel, pVis ) \
			CHOICE_BUTTON_EX( itemNum, plabel, play "mouse_click"; execnow "set selected_weapon_class "ptext"; set ui_weapon_class_selected "plabel; open CAC_PREFIX"popup_cac_"ptext"2";, name ptext ) \
			CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( pVis ) )		

		LOCAL_WEAPON_CLASS2( 1, "assault", "@MPUI_ASSAULT_RIFLES", statRangeAnyBitsSet( 3020, 3026, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS2( 2, "SMG", "@MPUI_SUB_MACHINE_GUNS", statRangeAnyBitsSet( 3010, 3019, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS2( 3, "shotgun", "@MPUI_SHOTGUNS", statRangeAnyBitsSet( 3070, 3079, WEAPON_NEW_BITMASK ) )
		LOCAL_WEAPON_CLASS2( 4, "sniper", "@MPUI_SNIPER_RIFLES", statRangeAnyBitsSet( 3060, 3069, WEAPON_NEW_BITMASK ) )	
	}
	
	// hackaround to work for PC's floating mouse unfocusing
	#undef  UI_FOCUSFIRST
	#ifdef PC
		#define UI_FOCUSFIRST "ui_primary_highlighted"
	#else
		#define UI_FOCUSFIRST "ui_fake_focus"
	#endif
	
	// primary weapon items
	#define LOCAL_MASTER_WEAPON_GROUP( suffix, pos, y_offset )\
	menuDef	{\
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_assault"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+20+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_M16"; set ui_inside_popup assault";, 0 )\
		LOCAL_WEAPON_INFO_WINDOW( "ui_primary_highlighted" )\
		LOCAL_WEAPON_ITEM( suffix, 1, "@WEAPON_M16", STAT_ASSAULT_M16, REF_M16, "ui_primary_highlighted", 3025 )\
		LOCAL_WEAPON_ITEM( suffix, 2, "@WEAPON_AK47", STAT_ASSAULT_AK47, REF_AK47, "ui_primary_highlighted", 3020 )\
		LOCAL_WEAPON_ITEM( suffix, 3, "@WEAPON_M4_CARBINE", STAT_ASSAULT_M4, REF_M4, "ui_primary_highlighted", 3026 )\
		LOCAL_WEAPON_ITEM( suffix, 4, "@WEAPON_G3", STAT_ASSAULT_G3, REF_G3, "ui_primary_highlighted", 3023 )\
		LOCAL_WEAPON_ITEM( suffix, 5, "@WEAPON_G36C", STAT_ASSAULT_G36C, REF_G36C, "ui_primary_highlighted", 3024 )\
	}\
	menuDef	{\
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_LMG"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+20+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_M249SAW"; set ui_inside_popup lmg";, 0 )\
		LOCAL_WEAPON_INFO_WINDOW( "ui_primary_highlighted" )\
		LOCAL_WEAPON_ITEM( suffix, 1, "@WEAPON_SAW", STAT_LMG_M249SAW, REF_M249SAW, "ui_primary_highlighted", 3081 )\
		LOCAL_WEAPON_ITEM( suffix, 2, "@WEAPON_RPD", STAT_LMG_RPD, REF_RPD, "ui_primary_highlighted", 3080 )\
		LOCAL_WEAPON_ITEM( suffix, 3, "@WEAPON_M60E4", STAT_LMG_M60E4, REF_M60E4, "ui_primary_highlighted", 3082 )\
	}\
	menuDef	{\
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_SMG"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+20+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_MP5";set ui_inside_popup smg";, 0 )\
		LOCAL_WEAPON_INFO_WINDOW( "ui_primary_highlighted" )\
		LOCAL_WEAPON_ITEM( suffix, 1, "@WEAPON_MP5", STAT_SMG_MP5, REF_MP5, "ui_primary_highlighted", 3010 )\
		LOCAL_WEAPON_ITEM( suffix, 2, "@WEAPON_SKORPION", STAT_SMG_SKORPION, REF_SKORPION, "ui_primary_highlighted", 3011 )\
		LOCAL_WEAPON_ITEM( suffix, 3, "@WEAPON_UZI", STAT_SMG_UZI, REF_UZI, "ui_primary_highlighted", 3012 )\
		LOCAL_WEAPON_ITEM( suffix, 4, "@WEAPON_AK74U", STAT_SMG_AK74U, REF_AK74U, "ui_primary_highlighted", 3013 )\
		LOCAL_WEAPON_ITEM( suffix, 5, "@WEAPON_P90", STAT_SMG_P90, REF_P90, "ui_primary_highlighted", 3014 )\
	}\
	menuDef	{\
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_shotgun"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+20+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_WINCHESTER1200"; set ui_inside_popup shotgun";, 0 )\
		LOCAL_WEAPON_INFO_WINDOW( "ui_primary_highlighted" )\
		LOCAL_WEAPON_ITEM( suffix, 1, "@WEAPON_WINCHESTER1200", STAT_SHOTGUN_WINCHESTER1200, REF_WINCHESTER1200, "ui_primary_highlighted", 3071 )\
		LOCAL_WEAPON_ITEM( suffix, 2, "@WEAPON_BENELLI", STAT_SHOTGUN_BENELLIM4, REF_BENELLIM4, "ui_primary_highlighted", 3070 )\
	}\
	menuDef	{\
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_sniper"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+20+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_M40A3"; set ui_inside_popup sniper";, 0 )\
		LOCAL_WEAPON_INFO_WINDOW( "ui_primary_highlighted" )\
		LOCAL_WEAPON_ITEM( suffix, 1, "@WEAPON_M40A3", STAT_SNIPER_M40A3, REF_M40A3, "ui_primary_highlighted", 3061 )\
		LOCAL_WEAPON_ITEM( suffix, 2, "@WEAPON_M21", STAT_SNIPER_M21, REF_M21, "ui_primary_highlighted", 3065 )\
		LOCAL_WEAPON_ITEM( suffix, 3, "@WEAPON_REMINGTON700", STAT_SNIPER_REMINGTON700, REF_REMINGTON700, "ui_primary_highlighted", 3064 )\
		LOCAL_WEAPON_ITEM( suffix, 4, "@WEAPON_BARRETT", STAT_SNIPER_BARRETT, REF_BARRETT, "ui_primary_highlighted", 3062 )\
	}
	
	// primary and second primary weapon selection popup menus
	LOCAL_MASTER_WEAPON_GROUP( "", 1, 0 )
	LOCAL_MASTER_WEAPON_GROUP( "2", 2, 4 )
	
	// ====================================================================================================
	// primary and second primary attachment selection ====================================================
	// ====================================================================================================
	#define LOCAL_ATTACHMENT_INFO_WINDOW( pslot ) \
		/* attachment information side frame*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) -4 280 CHOICE_POPUP_HEIGHT( 7 ) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 1, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "camo" ); ) \
		LOADOUT_PLATING_RAW( CHOICE_POPUP_WIDTH 8, -2, 0, 280, (CHOICE_POPUP_HEIGHT( 7 )-4), CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN visible when( dvarString(ui_inside_popup) != "camo" );, 0, 0 ) \
		/* attachment image*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 30 256 48 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "gradient_bottom", 1 1 1 0.3, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "camo" ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+182) 6 64 64 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, tablelookup("mp/attachmentTable.csv",4,dvarString(ui_attachment_highlighted),6), 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "camo" ); ) \
		/* attachment title*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 58 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmentTable.csv",4,dvarString(ui_attachment_highlighted),3), TEXTSIZE_DEFAULT, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( dvarString(ui_inside_popup) != "camo" ); ) \
		/* attachment desc unlocked*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmentTable.csv",4,dvarString(ui_attachment_highlighted),7), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, CHOICE_TEXTCOLOR visible when( stat( stat( pslot-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_attachment_highlighted), 10 ) ) ); )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmentTable.csv",4,dvarString(ui_attachment_highlighted),8), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( (  stat( stat( pslot-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_attachment_highlighted), 10 ) ) ) == 0 ); )\
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 0 (CHOICE_SIZE_Y*2) CHOICE_SIZE_Y CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "specialty_locked", 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( ( stat( stat( pslot-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_attachment_highlighted), 10 ))) == 0 && dvarString(ui_inside_popup) != "camo" ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( 0 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", 0.15 0.15 0.17 1, 0, 2, CHOICE_POPUP_BORDER_COLOR )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( 8 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, -32 0, "@"+tablelookup("mp/statstable.csv",0,stat(pslot-1),3), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_RIGHT, CHOICE_TEXTCOLOR )\
		/* pointer icon */ \
		PREPROC_SHADER_DRAW_ALIGNED( 200 -16 16 8 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "hitech_arrow_right", 0.55 0.95 0.55 0.7, 0, 2, CHOICE_POPUP_BORDER_COLOR )

	#define LOCAL_ATTACHMENT_ACTION( pslot, pnum, paction, pname ) \
		paction; \
		statsetusingtable( pslot, pnum ); \
		scriptMenuResponse "loadout_primary_attachment:"pname; \
		open CAC_PREFIX"popup_cac_camo";

	#define LOCAL_ATTACHMENT_ACTION2( pslot, pnum, paction, pname ) \
		paction; \
		statsetusingtable( pslot, pnum );\
		scriptMenuResponse "loadout_secondary_attachment:"pname; \
		PREPROC_ATTACH_CLOSEALL

    #define LOCAL_ATTACHMENT_ITEM( itemNum, p_setstat, p_numref, pname, paction, ptype, statDvar, bitMask )\
        CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, "@"+tablelookup("mp/attachmentTable.csv",4,pname,3), LOCAL_ATTACHMENT_ACTION( p_setstat, p_numref, paction, pname );CLEAR_NEW( statDvar, bitMask );, execnow "set ui_attachment_highlighted "pname;, CLEAR_NEW( statDvar, bitMask );, when( ptype == "primary" && ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ) ) ) ), ptype == "primary" && ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ) ) ) ) \
        CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, "@"+tablelookup("mp/attachmentTable.csv",4,pname,3), LOCAL_ATTACHMENT_ACTION2( p_setstat, p_numref, paction, pname );CLEAR_NEW( statDvar, bitMask );, execnow "set ui_attachment_highlighted "pname;, CLEAR_NEW( statDvar, bitMask );, when( ptype == "secondary" && ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 )) ) ), ptype == "secondary" && ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ) ) ) ) \
        CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set ui_attachment_highlighted "pname, ;, when( ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ))) == 0 ); ) \
        CHOICE_DBUTTON_VIS( itemNum, "@"+tablelookup("mp/attachmentTable.csv",4,pname,3), when( ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ))) == 0 ); ) \
        CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( ( int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 )) != 1 ) && (stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ))<<16 )) ) \
        CHOICE_NEWICON_VIS( itemNum, "specialty_locked", when( ( stat( stat( p_setstat-1 )+3000 ) & int(tablelookup( "mp/attachmenttable.csv", 9, p_numref, 10 ))) == 0 ); )

	// hackaround to work for PC's floating mouse unfocusing
	#undef  UI_FOCUSFIRST
	#ifdef PC
		#define UI_FOCUSFIRST "ui_attachment_highlighted"
	#else
		#define UI_FOCUSFIRST "ui_fake_focus"
	#endif
		
	// primary and second primary attachment items
	#define LOCAL_MASTER_ATTACHMENT_GROUP( stat_slot, suffix, pos, ptype, y_offset, statDvar )\
	menuDef { /* assault attachments*/ \
        IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_assault"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+44+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )\
        onClose{execnow "set ui_inside_popup 0";} \
        LOCAL_ATTACHMENT_INFO_WINDOW( stat_slot )\
        LOCAL_ATTACHMENT_ITEM( 1, stat_slot, NUM_NONE, "none", PERK1_RESET, ptype, statDvar, 0 )\
        LOCAL_ATTACHMENT_ITEM( 2, stat_slot, NUM_SUPPRESSOR, "silencer", PERK1_RESET, ptype, statDvar, SILENCER_NEW_BITMASK )\
        LOCAL_ATTACHMENT_ITEM( 3, stat_slot, NUM_REFLEX, "reflex", PERK1_RESET, ptype, statDvar, REDDOT_NEW_BITMASK )\
        LOCAL_ATTACHMENT_ITEM( 4, stat_slot, NUM_ACOG, "acog", PERK1_RESET, ptype, statDvar, ACOG_NEW_BITMASK )\
    }\
    menuDef { /* LMG attachments*/\
        IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_LMG"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+44+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )\
        onClose{execnow "set ui_inside_popup 0";} \
        LOCAL_ATTACHMENT_INFO_WINDOW( stat_slot )\
        LOCAL_ATTACHMENT_ITEM( 1, stat_slot, NUM_NONE, "none", PERK1_RESET, ptype, statDvar, 0 )\
        LOCAL_ATTACHMENT_ITEM( 2, stat_slot, NUM_REFLEX, "reflex", PERK1_RESET, ptype, statDvar, REDDOT_NEW_BITMASK )\
        LOCAL_ATTACHMENT_ITEM( 3, stat_slot, NUM_GRIP, "grip", play "mouse_click"; execnow "statset "CAC_S_SPECIALTY_EQUIPMENT" 193", ptype, statDvar, GRIP_NEW_BITMASK )\
        LOCAL_ATTACHMENT_ITEM( 4, stat_slot, NUM_ACOG, "acog", PERK1_RESET, ptype, statDvar, ACOG_NEW_BITMASK )\
    }\
    menuDef    { /* SMG attachments*/\
        IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_SMG"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+44+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )\
        onClose{execnow "set ui_inside_popup 0";} \
        LOCAL_ATTACHMENT_INFO_WINDOW( stat_slot )\
        LOCAL_ATTACHMENT_ITEM( 1, stat_slot, NUM_NONE, "none", PERK1_RESET, ptype, statDvar, 0 )\
        LOCAL_ATTACHMENT_ITEM( 2, stat_slot, NUM_SUPPRESSOR, "silencer", PERK1_RESET, ptype, statDvar, SILENCER_NEW_BITMASK )\
        LOCAL_ATTACHMENT_ITEM( 3, stat_slot, NUM_REFLEX, "reflex", PERK1_RESET, ptype, statDvar, REDDOT_NEW_BITMASK )\
    }\
    menuDef { /* shotgun attachments*/\
        IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_shotgun"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+44+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )\
        onClose{execnow "set ui_inside_popup 0";} \
        LOCAL_ATTACHMENT_INFO_WINDOW( stat_slot )\
        LOCAL_ATTACHMENT_ITEM( 1, stat_slot, NUM_NONE, "none", PERK1_RESET, ptype, statDvar, 0 )\
        LOCAL_ATTACHMENT_ITEM( 2, stat_slot, NUM_GRIP, "grip", play "mouse_click"; execnow "statset "CAC_S_SPECIALTY_EQUIPMENT" 193", ptype, statDvar, GRIP_NEW_BITMASK )\
    }\
    menuDef    { /* sniper attachments*/\
        IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_sniper"suffix, 7, (CHOICE_X( pos )-2), (CHOICE_Y( pos )+44+y_offset), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )\
        onClose{execnow "set ui_inside_popup 0";} \
        LOCAL_ATTACHMENT_INFO_WINDOW( stat_slot )\
        LOCAL_ATTACHMENT_ITEM( 1, stat_slot, NUM_NONE, "none", PERK1_RESET, ptype, statDvar, 0 )\
    }
	// side arm attachment selection ======================================================================
	menuDef { /* pistol attachments*/
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"attachment_popup_pistol", 7, (CHOICE_X( 2 )-2), (CHOICE_Y( 2 )+24), ;, execnow "set "UI_FOCUSFIRST" "REF_ATTACHMENT_NONE"; set ui_inside_popup attachment";, 0 )
		onClose{execnow "set ui_inside_popup 0";}
		LOCAL_ATTACHMENT_INFO_WINDOW( CAC_SECONDARY_ATTACHMENT )
		
		// Vedd le az idézőjelet a secondary-ről:
		LOCAL_ATTACHMENT_ITEM( 1, CAC_SECONDARY_ATTACHMENT, NUM_NONE, "none", PERK1_RESET, secondary, "ui_secondary_weapon", 0 )
		LOCAL_ATTACHMENT_ITEM( 2, CAC_SECONDARY_ATTACHMENT, NUM_SUPPRESSOR, "silencer", PERK1_RESET, secondary, "ui_secondary_weapon", SILENCER_NEW_BITMASK )
	}
	// primary and second primary attachment popup menus
	LOCAL_MASTER_ATTACHMENT_GROUP( CAC_PRIMARY_ATTACHMENT, "", 1, "primary", 0, "ui_primary_weapon" )
	LOCAL_MASTER_ATTACHMENT_GROUP( CAC_SECONDARY_ATTACHMENT, "2", 2, "secondary", 4, "ui_secondary_weapon" )

	// ====================================================================================================
	// side arm selection ===============================================================================
	// ====================================================================================================
	#define LOCAL_SIDEARM_INFO_WINDOW( highlight_dvar ) \
		/* sidearm information side frame*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) -4 280 CHOICE_POPUP_HEIGHT( 7 ) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 1, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" ); ) \
		LOADOUT_PLATING_RAW( CHOICE_POPUP_WIDTH 8, -2, 0, 280, (CHOICE_POPUP_HEIGHT( 7 )-4), CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN visible when( dvarString(ui_inside_popup) != "attachment" );, 0, 0 )\
		/* sidearm image*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 30 256 48 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "gradient_bottom", 1 1 1 0.3, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+138) -12 90 90 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),6), 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" ); ) \
		/* sidearm title*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 58 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),3), TEXTSIZE_DEFAULT, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( dvarString(ui_inside_popup) != "attachment" ); ) \
		/* sidearm desc unlocked*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),7), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, CHOICE_TEXTCOLOR visible when( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ); )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),10), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( ( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ) == 0 ); )\
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 0 (CHOICE_SIZE_Y*2) CHOICE_SIZE_Y CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "specialty_locked", 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( ( stat(int(tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),1))) & 1 ) == 0 ); ) \

	#define LOCAL_SIDEARM_ACTION( pstat, weapRef ) \
			execnow "statset "CAC_S_SECONDARY_ATTACHMENT" 0" \
			play "mouse_click"; \
			scriptMenuResponse "loadout_secondary:"weapRef;\
			statsetusingtable( CAC_SECONDARY, pstat ); \
			statsetusingtable( CAC_SECONDARY_STORAGE, tableLookup( "mp/statstable.csv", 4, weapRef, 1 ) ); \
			execNow "statGetInDvar "CAC_S_SECONDARY_STORAGE" ui_secondary_weapon"; \
			CLEAR_NEW( ui_secondary_weapon, WEAPON_NEW_BITMASK ); \
			PREPROC_ATTACH_CLOSEALL

	#define LOCAL_SIDEARM_ACTION2( pstat, weapRef ) \		
			execNow "set ui_perk1_slot 0"; \
			execnow "statset "CAC_S_SECONDARY_ATTACHMENT" 0" \
			statsetusingtable( CAC_SECONDARY, pstat ); \
			statsetusingtable( CAC_SECONDARY_STORAGE, tableLookup( "mp/statstable.csv", 4, weapRef, 1 ) ); \
			execNow "statGetInDvar "CAC_S_SECONDARY_STORAGE" ui_secondary_weapon"; \
			CLEAR_NEW( ui_secondary_weapon, WEAPON_NEW_BITMASK ); \
			play "mouse_click"; \
			scriptMenuResponse "loadout_secondary:"weapRef;\
			uiScript openMenuOnDvar "selected_weapon_class" pistol CAC_PREFIX"attachment_popup_pistol";
		
	#define LOCAL_SIDEARM_ITEM( itemNum, weaponName, weaponStat, weaponRef, highlight_dvar )\
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_SIDEARM_ACTION( weaponStat, weaponRef ), execnow "set "highlight_dvar" "weaponRef, CLEAR_WEAP_NEW( weaponRef );, \
		when( ( stat(int(weaponStat)+3000) & 1 ) && ( "deserteagle"==tablelookup("mp/statstable.csv", 0, weaponStat , 4) || "deserteaglegold"==tablelookup("mp/statstable.csv", 0, weaponStat , 4) ) );, \
			  ( stat(int(weaponStat)+3000) & 1 ) && ( "deserteagle"==tablelookup("mp/statstable.csv", 0, weaponStat , 4) || "deserteaglegold"==tablelookup("mp/statstable.csv", 0, weaponStat , 4) ) ) \
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_SIDEARM_ACTION2( weaponStat, weaponRef ), execnow "set "highlight_dvar" "weaponRef, CLEAR_WEAP_NEW( weaponRef );, \
		when( ( stat(int(weaponStat)+3000) & 1 ) && "deserteagle"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) && "deserteaglegold"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) );, \
			  ( stat(int(weaponStat)+3000) & 1 ) && "deserteagle"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) && "deserteaglegold"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) ) \
		CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "weaponRef, ;, when( ( stat(int(weaponStat)+3000) & 1 ) == 0 && "deserteaglegold"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) ); ) \
		CHOICE_DBUTTON_VIS( itemNum, weaponName, when( ( stat(int(weaponStat)+3000) & 1 ) == 0 && "deserteaglegold"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) ); ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( ( (stat(int(weaponStat)+3000)) & (NEW_WEAPON_BITMASK+NEW_ITEMS_MASK) ) > 0 ) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_locked", when( ( stat(int(weaponStat)+3000) & 1 ) == 0 && "deserteaglegold"!=tablelookup("mp/statstable.csv", 0, weaponStat , 4) ); )
	// side arm selection popup menu
	
	// hackaround to work for PC's floating mouse unfocusing
	#undef  UI_FOCUSFIRST
	#ifdef PC
		#define UI_FOCUSFIRST "ui_sidearm_highlighted"
	#else
		#define UI_FOCUSFIRST "ui_fake_focus"
	#endif
	
	menuDef	
	{
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_secondary", 7, (CHOICE_X( 2 )-2), (CHOICE_Y( 2 )), ;, execnow "set "UI_FOCUSFIRST" "REF_M9BERETTA"; set selected_weapon_class pistol; set ui_inside_popup pistol";, 1 )
		onClose{execnow "set ui_inside_popup 0";}
		LOCAL_SIDEARM_INFO_WINDOW( "ui_sidearm_highlighted" )
		LOCAL_SIDEARM_ITEM( 1, "@WEAPON_BERETTA", STAT_PISTOL_M9BERETTA, REF_M9BERETTA, "ui_sidearm_highlighted" )
		LOCAL_SIDEARM_ITEM( 2, "@WEAPON_USP", STAT_PISTOL_USP, REF_USP, "ui_sidearm_highlighted" )
		LOCAL_SIDEARM_ITEM( 3, "@WEAPON_COLT1911", STAT_PISTOL_COLT45, REF_COLT45, "ui_sidearm_highlighted" )
		LOCAL_SIDEARM_ITEM( 4, "@WEAPON_DESERTEAGLE", STAT_PISTOL_DESERTEAGLE, REF_DESERTEAGLE, "ui_sidearm_highlighted" )
		LOCAL_SIDEARM_ITEM( 5, "@WEAPON_DESERTEAGLEGOLD", STAT_PISTOL_DESERTEAGLEGOLD, REF_DESERTEAGLEGOLD, "ui_sidearm_highlighted" )
	}

	// ====================================================================================================
	// camo skin selection ================================================================================
	// ====================================================================================================
	#define LOCAL_CAMO_INFO_WINDOW( highlight_dvar ) \
		/* camo information side frame*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) -4 280 CHOICE_POPUP_HEIGHT( 8 ) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 1, 2, CHOICE_POPUP_BORDER_COLOR ) \
		LOADOUT_PLATING_RAW( CHOICE_POPUP_WIDTH 8, -2, 0, 280, (CHOICE_POPUP_HEIGHT( 8 )-4), CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0, 0 )\
		/* camo image*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 30 256 48 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "gradient_bottom", 1 1 1 0.3, 0, 2, CHOICE_POPUP_BORDER_COLOR ) \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+184) 0 64 64 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, tablelookup("mp/attachmentTable.csv",4,dvarString(highlight_dvar),6), 1 1 1 0.75, 1, 7, 0.2 0.2 0.225 1 ) \
		/* camo title*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 58 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmenttable.csv",4,dvarString(highlight_dvar),3), TEXTSIZE_DEFAULT, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE ) \
		/* camo desc unlocked*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmenttable.csv",4,dvarString(highlight_dvar),7), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, CHOICE_TEXTCOLOR visible when( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_camo_highlighted), 10 )) ); )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 80 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/attachmenttable.csv",4,dvarString(highlight_dvar),8), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE visible when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_camo_highlighted), 10 )) ) == 0 ); )\
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 0 (CHOICE_SIZE_Y*2) CHOICE_SIZE_Y CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "specialty_locked", 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 4, dvarString(ui_camo_highlighted), 10 )) ) == 0 ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( 0 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", 0.15 0.15 0.17 1, 0, 2, CHOICE_POPUP_BORDER_COLOR )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( 8 -24 (CHOICE_POPUP_WIDTH-4) 22 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, -32 0, "@"+tablelookup("mp/attachmenttable.csv",9,stat(CAC_PRIMARY_ATTACHMENT),3), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_RIGHT, CHOICE_TEXTCOLOR )\
		/* pointer icon */ \
		PREPROC_SHADER_DRAW_ALIGNED( 200 -16 16 8 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "hitech_arrow_right", 0.55 0.95 0.55 0.7, 0, 2, CHOICE_POPUP_BORDER_COLOR )

	#define LOCAL_CAMO_ACTION( pnum, camoRef ) \
		play "mouse_click"; \
		statsetusingtable( CAC_CAMO, pnum ); \
		scriptMenuResponse "loadout_camo:"camoRef; \
		execnow "set loadout_camo "camoRef; \
		PREPROC_ATTACH_CLOSEALL
		
	#define LOCAL_CAMO_ITEM( itemNum, camoName, pnum, camoRef, highlight_dvar, bitMask, visArg, refName )\
		CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, camoName, LOCAL_CAMO_ACTION( pnum, refName );CLEAR_NEW( "ui_primary_weapon", bitMask );, execnow "set "highlight_dvar" "camoRef, CLEAR_NEW( "ui_primary_weapon", bitMask );, \
		when( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) && ( visArg ) );, \
			  stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) && ( visArg ) ) \
		CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "camoRef, ;, when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) ) == 0 && ( visArg ) ); ) \
		CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "camoRef, ;, when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) ) == 0 && ( visArg ) ); ) \
		CHOICE_DBUTTON_VIS( itemNum, camoName, when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) ) == 0 && ( visArg ) ); ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_new", when( ( int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) != 1 ) && \
		( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 ))<<16 ) && ( visArg ) ) ) \
		CHOICE_NEWICON_VIS( itemNum, "specialty_locked", when( ( stat(stat(CAC_PRIMARY)+3000) & int(tablelookup( "mp/attachmenttable.csv", 11, pnum, 10 )) ) == 0 && ( visArg ) ); )
				
	#define GOLDEN_VIS_CONDITION \
		tablelookup("mp/statstable.csv",1,dvarString(ui_primary_weapon),4)==REF_UZI ||\
		tablelookup("mp/statstable.csv",1,dvarString(ui_primary_weapon),4)==REF_AK47 ||\
		tablelookup("mp/statstable.csv",1,dvarString(ui_primary_weapon),4)==REF_DRAGUNOVSVD ||\
		tablelookup("mp/statstable.csv",1,dvarString(ui_primary_weapon),4)==REF_BENELLIM4 ||\
		tablelookup("mp/statstable.csv",1,dvarString(ui_primary_weapon),4)==REF_M60E4
	
	// BRENT ATTENTION
	#define PRESTIGE_VIS_CONDITION \
		0

	// hackaround to work for PC's floating mouse unfocusing
	#undef  UI_FOCUSFIRST
	#ifdef PC
		#define UI_FOCUSFIRST "ui_camo_highlighted"
	#else
		#define UI_FOCUSFIRST "ui_fake_focus"
	#endif
	
	// camo skin selection for primary weapon popup menu
	#define LOCAL_CAMO_GROUP( prefix, onLeave )\
	menuDef    \
	{ \
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_camo"prefix, 8, (CHOICE_X( 1 )-2), (CHOICE_Y( 1 )+68), ;, execnow "set "UI_FOCUSFIRST" "REF_CAMO_NONE"; set ui_inside_popup camo";, 0 )\
		onClose{onLeave;} \
		LOCAL_CAMO_INFO_WINDOW( "ui_camo_highlighted" )\
		/* Itt hozzáadtuk a szöveges neveket a végére (pl. "none", "desert") */ \
		LOCAL_CAMO_ITEM( 1, "@MPUI_NONE", CAMO_NONE, REF_CAMO_NONE, "ui_camo_highlighted", 0, 1, "none" )\
		LOCAL_CAMO_ITEM( 2, "@MPUI_DESERT", CAMO_BROCKHUARD, REF_BROCKHUARD, "ui_camo_highlighted", DESERT_NEW_BITMASK, 1, "desert" )\
		LOCAL_CAMO_ITEM( 3, "@MPUI_WOODLAND", CAMO_BUSHDWELLER, REF_BUSHDWELLER, "ui_camo_highlighted", WOODLAND_NEW_BITMASK, 1, "woodland" )\
		LOCAL_CAMO_ITEM( 4, "@MPUI_DIGITAL", CAMO_BLACKWHITEMARPAT, REF_BLACKWHITEMARPAT, "ui_camo_highlighted", DIGITAL_NEW_BITMASK, 1, "digital" )\
		LOCAL_CAMO_ITEM( 5, "@MPUI_RED_TIGER", CAMO_TIGERRED, REF_TIGERRED, "ui_camo_highlighted", REDTIGER_NEW_BITMASK, 1, "red_tiger" )\
		LOCAL_CAMO_ITEM( 6, "@MPUI_BLUE_TIGER", CAMO_STAGGER, REF_STAGGER, "ui_camo_highlighted", BLUETIGER_NEW_BITMASK, 1, "blue_tiger" )\
		LOCAL_CAMO_ITEM( 7, "@MPUI_GOLDEN", CAMO_GOLDEN, REF_GOLDEN, "ui_camo_highlighted", GOLDEN_NEW_BITMASK, GOLDEN_VIS_CONDITION, "gold" )\
		LOCAL_CAMO_ITEM( 8, "@MPUI_PRESTIGE", CAMO_PRESTIGE, REF_PRESTIGE, "ui_camo_highlighted", PRESTIGE_NEW_BITMASK, PRESTIGE_VIS_CONDITION, "prestige" )\
	}
	// camo skin selection for primary weapon popup menu
	LOCAL_CAMO_GROUP( "", execnow "set ui_inside_popup attachment" )
	// camo skin selection for primary weapon without attachments
	LOCAL_CAMO_GROUP( "2", execnow "set ui_inside_popup 0"; close CAC_PREFIX"attachment_popup_fake" )
	
	
	// ====================================================================================================
	// special grenade selection ==========================================================================
	// ====================================================================================================
	#define LOCAL_SGRENADE_INFO_WINDOW( highlight_dvar ) \
		/* special grenade information side frame*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) -4 280 CHOICE_POPUP_HEIGHT(6) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 1, 2, CHOICE_POPUP_BORDER_COLOR ) \
		LOADOUT_PLATING_RAW( CHOICE_POPUP_WIDTH 8, -2, 0, 280, (CHOICE_POPUP_HEIGHT(6)-4), CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN visible when( dvarString(ui_inside_popup) != "attachment" );, 0, 0 )\
		/*PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH-6) (CHOICE_POPUP_HEIGHT(3)-4) 2 (CHOICE_POPUP_HEIGHT(6)-CHOICE_POPUP_HEIGHT(3)+2) CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "white", CHOICE_POPUP_BORDER_COLOR, 0, 2, CHOICE_POPUP_BORDER_COLOR )*/ \
		/* special grenade image*/ \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+6) 6 256 48 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "gradient_bottom", 1 1 1 0.3, 0, 2, CHOICE_POPUP_BORDER_COLOR visible when( dvarString(ui_inside_popup) != "attachment" ); ) \
		PREPROC_SHADER_DRAW_ALIGNED( (CHOICE_POPUP_WIDTH+178) -6 64 64 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, tablelookup("mp/statsTable.csv",4,dvarString(highlight_dvar),6), 1 1 1 1, 0, 2, CHOICE_POPUP_BORDER_COLOR ) \
		/* special grenade title*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 34 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),3), TEXTSIZE_DEFAULT, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE ) \
		/* special grenade desc unlocked*/\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 56 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),7), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, CHOICE_TEXTCOLOR \
		visible when( ( ( dvarString(ui_sgrenade_highlighted)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ) == 0 ); )\
		PREPROC_TEXT_DRAW_ALIGNED_EXP( (CHOICE_POPUP_WIDTH+10) 56 256 20 CHOICE_HORIZONTAL_ALIGN CHOICE_VERTICAL_ALIGN, 0 0, "@"+tablelookup("mp/statstable.csv",4,dvarString(highlight_dvar),10), TEXTSIZE_SMALL, 0, 0, ITEM_ALIGN_MIDDLE_LEFT, COLOR_TITLE \
		visible when( (dvarString(ui_sgrenade_highlighted)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ); )

	#define LOCAL_SGRENADE_ACTION( pstat, weapRef ) \
		play "mouse_click"; \
		statsetusingtable( CAC_SPECIAL_GRENADE, pstat ); \
		scriptMenuResponse "loadout_grenade:"weapRef; \
		close CAC_PREFIX"popup_cac_extra"
		
	#define LOCAL_SGRENADE_ITEM( itemNum, weaponName, weaponStat, weaponRef, highlight_dvar )\
        CHOICE_BUTTON_FOCUS_VIS_ADV( itemNum, weaponName, LOCAL_SGRENADE_ACTION( weaponStat, weaponRef ), execnow "set "highlight_dvar" "weaponRef, ;, \
        when( ( (tableLookup("mp/statsTable.csv",0,weaponStat,4)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ) == 0 );, \
              ( (tableLookup("mp/statsTable.csv",0,weaponStat,4)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ) == 0 ) \
        CHOICE_BUTTON_FOCUS_VIS_NOHI( itemNum, "", ;, execnow "set "highlight_dvar" "weaponRef, ;, when( (tableLookup("mp/statsTable.csv",0,weaponStat,4)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ); ) \
        CHOICE_DBUTTON_VIS( itemNum, weaponName, when( (tableLookup("mp/statsTable.csv",0,weaponStat,4)=="smoke_grenade") && (tableLookup("mp/statsTable.csv",0,stat(CAC_SPECIALTY_EQUIPMENT),4)=="specialty_specialgrenade") ); )
	
	// hackaround to work for PC's floating mouse unfocusing
	#undef  UI_FOCUSFIRST
	#ifdef PC
		#define UI_FOCUSFIRST "ui_sgrenade_highlighted"
	#else
		#define UI_FOCUSFIRST "ui_fake_focus"
	#endif
	
	// special grenade selection popup menus
	menuDef	
	{
		IMPROVED_POPUP_SETUP_ONOPEN( CAC_PREFIX"popup_cac_extra", 6, (CHOICE_X( 3 )-2), (CHOICE_Y( 3 )+4), ;, execnow "set "UI_FOCUSFIRST" "REF_FLASH_X1"; set ui_inside_popup sgrenade";, 1 )
		onClose{ execnow "set ui_inside_popup 0"; }
		LOCAL_SGRENADE_INFO_WINDOW( "ui_sgrenade_highlighted" )
		LOCAL_SGRENADE_ITEM( 1, "@WEAPON_FLASH_GRENADE", STAT_FLASH_X1, REF_FLASH_X1, "ui_sgrenade_highlighted" )
		LOCAL_SGRENADE_ITEM( 2, "@WEAPON_STUN_GRENADE", STAT_CONCUSSION_X1, REF_CONCUSSION_X1, "ui_sgrenade_highlighted" )
		LOCAL_SGRENADE_ITEM( 3, "@WEAPON_SMOKE_GRENADE", STAT_SMOKE_X1, REF_SMOKE_X1, "ui_sgrenade_highlighted" )
	}
	
