#include "item.h"
#include "map.h"

Item_info repository[] = {
    {.name = "Girards' Elixir of Spelling",
     .descr = "1",
     .sprite = NULL},
    {.name = "Tantrum Potion",
     .descr = "2",
     .sprite = NULL},
    {.name = "Monkey Tears",
     .descr = "3",
     .sprite = NULL},
    {.name = "Quoin",
     .descr = "4",
     .sprite = NULL},
    {.name = "Dagger",
     .descr = "5",
     .sprite = NULL}};

void load_item_sprites(PALETTE pal)
{
    BITMAP *atlas = load_tga("testart.tga", 0);

    repository[0].sprite = create_bitmap(16, 16);
    repository[1].sprite = create_bitmap(16, 16);
    repository[2].sprite = create_bitmap(16, 16);
    repository[3].sprite = create_bitmap(16, 16);
    repository[4].sprite = create_bitmap(16, 16);
  
    blit(atlas, repository[0].sprite, 32, 0, 0, 0, 16, 16);
    blit(atlas, repository[1].sprite, 48, 0, 0, 0, 16, 16);
    blit(atlas, repository[2].sprite, 64, 0, 0, 0, 16, 16);
    blit(atlas, repository[3].sprite, 80, 0, 0, 0, 16, 16);
    blit(atlas, repository[4].sprite, 96, 0, 0, 0, 16, 16);
}

void put_item(Maptile *tile, unsigned char ref, unsigned char meta, unsigned char next) {

}