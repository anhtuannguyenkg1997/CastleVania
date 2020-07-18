 #pragma once

// OBJECT TYPE ID
#define OBJECT_TYPE_ITEMS           2           //Item bình thường
#define OBJECT_TYPE_HIDDEN_OBJECT	22          //Item ẩn


//STATE
#define ITEM_ACTIVE                 0            //Active Item (bắt đầu đếm thời gian biến mất)


//TYPE ITEMS
#define NORMAL_ITEMS	            1            //Item thường (NORMAL ITEMS)
#define SPECIAL_ITEMS	            2            //Item đặc biệt (SPECIAL ITEMS & WEAPON ITEMS)



// ITEM TYPE ID
//ITEM RANDOM
#define ITEM_RANDOM                 1000        //ID Item này chỉ để biết rằng nó sẽ random ra 1 item nào đó

//NORMAL ITEMS
#define ITEM_MORNINGSTAR			200
#define ITEM_SMALLHEART		        201
#define ITEM_BIGHEART		        202
#define ITEM_MONEYBAG_RED	        203
#define ITEM_MONEYBAG_PURPLE	    204
#define ITEM_MONEYBAG_WHITE         205

//SPECIAL ITEMS
#define ITEM_ROSARY			        206
#define ITEM_INVISIBILITYPOTION	    207
#define ITEM_DOUBLESHOT		        208
#define ITEM_TRIPLESHOT		        209

//WEAPON ITEMS
#define ITEM_DAGGER			        214
#define	ITEM_AXE				    215
#define ITEM_HOLYWATER		        216
#define ITEM_BOOMERANG		        217
#define	ITEM_STOPWATCH		        218



//ONLY WHEN YOU DEFEAT A BOSS
#define ITEM_MAGIC_CRYSTAL	        219

//HIDDEN ITEMS
#define ITEM_PORKCHOP		        210
#define ITEM_MONEYBAG               211
#define ITEM_CHEST                  212
#define ITEM_DIADEM                 213








//WIDTH & HEIGHT
//Cần tăng height của tất cả item 5px . 
//Có thể item sẽ va chạm với simon từ trước nên sweept k bắt đc mà phải dùng Broadcast test
//Nếu không tăng height nó sẽ chìm xuống 1 chút so với vì Broadcast test không đẩy Item ra khỏi Brick
#define ITEM_MORNINGSTAR_WIDTH			    32
#define ITEM_MORNINGSTAR_HEIGHT			    37

#define ITEM_SMALLHEART_WIDTH		        16
#define ITEM_SMALLHEART_HEIGHT		        30

#define ITEM_BIGHEART_WIDTH		            24
#define ITEM_BIGHEART_HEIGHT		        27

#define ITEM_MONEYBAG_WIDTH	                30
#define ITEM_MONEYBAG_HEIGHT                35

#define ITEM_ROSARY_WIDTH			        32
#define ITEM_ROSARY_HEIGHT			        39

#define ITEM_INVISIBILITYPOTION_WIDTH	    26
#define ITEM_INVISIBILITYPOTION_HEIGHT	    36

#define ITEM_DOUBLE_TRIPLE_SHOT_WIDTH		28
#define ITEM_DOUBLE_TRIPLE_SHOT_HEIGHT		30

#define ITEM_PORKCHOP_WIDTH		            32
#define ITEM_PORKCHOP_HEIGHT		        29

#define ITEM_CHEST_WIDTH                    32
#define ITEM_CHEST_HEIGHT                   27

#define ITEM_DIADEM_WIDTH                   30
#define ITEM_DIADEM_HEIGHT                  32

#define ITEM_DAGGER_WIDTH			        32
#define ITEM_DAGGER_HEIGHT			        21

#define	ITEM_AXE_WIDTH				        30
#define	ITEM_AXE_HEIGHT				        33

#define ITEM_HOLYWATER_WIDTH		        32
#define ITEM_HOLYWATER_HEIGHT		        37

#define ITEM_BOOMERANG_WIDTH		        30
#define ITEM_BOOMERANG_HEIGHT		        33

#define	ITEM_STOPWATCH_WIDTH		        30
#define	ITEM_STOPWATCH_HEIGHT		        37

#define	ITEM_MAGICCRYSTAL_WIDTH		        31
#define	ITEM_MAGICCRYSTAL_HEIGHT		    37




//ITEM PROPERTIES
#define ITEM_FALLING_SPEED			    0.25f       //Vận tốc rơi của Item thông thường
#define ITEM_MAGICCRYSTAL_FALLING_SPEED 0.15f       //Vận tốc rơi của Magic crystal




//SMALL HEART SPECIAL DEFINITION
#define ITEM_SMALLHEART_MAXVX			0.09f       //Vận tốc cực đại của Small heart di chuyển theo chiều ngang (trục x)
#define ITEM_SMALLHEART_FALLING_SPEED	0.05f       //Vận tốc rơi của Small heart (nhỏ hơn các item khác để nó rơi chậm xuống hơn)
#define ITEM_SMALLHEART_DELTAVX 	    0.01f       //Biến thiên vận tốc của Small Heart (để quả tim rơi lòng vòng trước khi chạm đất)
#define ITEM_SMALLHEART_VALUE           1           //Thêm 1 đơn vị vũ khí cho Simon


//BIG HEART SPECIAL DEFINITION
#define ITEM_BIGHEART_VALUE             5           //Thêm 5 đơn vị vũ khí cho Simon


//PORKCHOP SPECIAL DEFINITION
#define ITEM_PORKCHOP_VALUE             6           //Thêm 6 HP cho Simon

//MAGIC CRYSTAL 
#define MAGICCRYSTAL_X_APPEAR 1264                  //Vị trí mà Magic crystal xuất hiện khi Boss bị tiêu diệt
#define MAGICCRYSTAL_Y_APPEAR 50


//TIMING
#define ITEM_TIME_DISAPEAR		        6000        //Thời gian item xuất hiện
#define ITEM_ROSARY_TIME                500         //Thời gian dùng Rosary Item
#define ITEM_INVISIBILITY_TIME          2000        //Thời gian dùng Invisibility Item


//SCORE SPECIAL DEFINITION
#define ITEM_CHEST_SCORE                2000        //Điểm khi ăn Rương
#define ITEM_DIADEM_SCORE               2000        //Điểm khi ăn Vương miện
#define ITEM_MONEYBAG_SCORE             1000        //Điểm khi túi tiền nhấp nháy
#define ITEM_MONEYBAG_PURPLE_SCORE      400         //Điểm khi túi tiền tím
#define ITEM_MONEYBAG_RED_SCORE         100         //Điểm khi túi tiền tím
#define ITEM_MONEYBAG_WHITE_SCORE       700         //Thêm 5 đơn vị vũ khí cho Simon