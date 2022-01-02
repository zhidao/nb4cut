#include <stdio.h>
#include <stdlib.h>

#if __STDC_VERSION__ < 199901L
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
#else
#include <stdint.h>
#endif

#define NB4_WIDTH          64
#define NB4_HEIGHT         80
#define NB4_NFACE         311 /* number of faces */
#define NB4_BPP             1 /* byte per pixel */

#define NB4_HEADERSIZE     54
#define NB4_INFOHEADERSIZE 40
#define NB4_BITCOUNT        ( NB4_BPP * 8 )
#define NB4_PPM          2834 /* pixels per meter */

#define NB4_NCOLOR        256

void nb4_write_val(FILE *fp, size_t size, uint32_t val)
{
  for( ; size>0; size--, val>>=8 )
    fputc( val & 0xff, fp );
}

void nb4cut_write_header(FILE *fp)
{
  uint32_t bpl, rest;
  size_t imgsize, offset;

  /* header charactor */
  fputc( 'B', fp );
  fputc( 'M', fp );
  /* image size calculation */
  if( ( rest = ( bpl = NB4_WIDTH * NB4_BPP ) % 4 ) ) bpl += 4 - rest; /* padding */
  imgsize = bpl * NB4_HEIGHT;
  offset = NB4_HEADERSIZE + NB4_NCOLOR * 4;
  /* file size */
  nb4_write_val( fp, 4, offset + imgsize );
  nb4_write_val( fp, 4, 0 ); /* reserved */
  nb4_write_val( fp, 4, offset ); /* data offset */
  /* info header size */
  nb4_write_val( fp, 4, NB4_INFOHEADERSIZE );
  nb4_write_val( fp, 4, NB4_WIDTH ); /* image width */
  nb4_write_val( fp, 4, NB4_HEIGHT ); /* image height */
  nb4_write_val( fp, 2, 1 ); /* number of plane */
  nb4_write_val( fp, 2, NB4_BITCOUNT ); /* bit count */
  nb4_write_val( fp, 4, 0 ); /* no compression */
  nb4_write_val( fp, 4, imgsize ); /* image size */
  nb4_write_val( fp, 4, NB4_PPM ); /* X pixels/meter */
  nb4_write_val( fp, 4, NB4_PPM ); /* Y pixels/meter */
  nb4_write_val( fp, 4, NB4_NCOLOR ); /* number of actually used colors */
  nb4_write_val( fp, 4, 0 ); /* number of important color */
}

void nb4cut_read_palette(FILE *fin, uint32_t palette[])
{
  int i;

  fseek( fin, NB4_HEADERSIZE, SEEK_SET );
  for( i=0; i<NB4_NCOLOR; i++ )
    if( fread( &palette[i], 4, 1, fin ) != 1 ) break;
}

void nb4cut_write_palette(FILE *fout, uint32_t palette[])
{
  int i;

  for( i=0; i<NB4_NCOLOR; i++ )
    fwrite( &palette[i], 4, 1, fout );
}

void nb4cut_write_pixel(FILE *fin, FILE *fout, int id)
{
  int32_t val;
  int i, j;

  if( id >= NB4_NFACE ){
    fprintf( stderr, "invalid face identifier %d/%d\n", id, NB4_NFACE );
    return;
  }
  fseek( fin, NB4_HEADERSIZE + NB4_NCOLOR*4 + ( NB4_NFACE - id - 1 ) * NB4_WIDTH * NB4_HEIGHT * NB4_BPP, SEEK_SET );
  for( i=0; i<NB4_HEIGHT; i++ )
    for( j=0; j<NB4_WIDTH; j++ ){
      if( fread( &val, NB4_BPP, 1, fin ) == 1 )
        fwrite( &val, NB4_BPP, 1, fout );
    }
}

FILE *nb4cut_open(char *filename, char *mode)
{
  FILE *fp;

  if( !( fp = fopen( filename, mode ) ) ){
    fprintf( stderr, "Cannot open file %s.\n", filename );
    return NULL;
  }
  return fp;
}

const char *facename[] = {
  "date_harumune",
  "date_terumune",
  "date_masamune",
  "date_shigezane",
  "katakura_kagetsuna",
  "",
  "mogami_yoshimori",
  "mogami_yoshiaki",
  "uesugi_kenshin",
  "uesugi_kagetora",
  "naoe_kanetsugu",
  "",
  "satake_yoshishige",
  "utsunomiya_hirotsuna",
  "",
  "yuuki_harutomo",
  "uesugi_norizane",
  "",
  "houjo_ujiyasu",
  "takeda_shingen",
  "imagawa_yoshimoto",
  "taigen_sessai",
  "obu_toramasa",
  "asahina_yasuyoshi",
  "tokugawa_ieyasu",
  "honganji_kousa",
  "asakura_souteki",
  "akechi_mitsuhide",
  "saito_dousan",
  "oda_nobunaga",
  "oda_nobuyuki",
  "hayashi_michikatsu",
  "mori_yoshinari",
  "azai_hisamasa",
  "isono_kazumasa",
  "rokkaku_yoshikata",
  "",
  "shima_sakon",
  "araki_murashige",
  "",
  "mouri_motonari",
  "ankokuji_ekei",
  "ouchi_yoshinaga",
  "sue_harukata",
  "",
  "otomo_sourin",
  "otomo_yoshimune",
  "tachibana_dosetsu",
  "ryuzouji_takanobu",
  "",
  "nabeshima_naoshige",
  "",
  "",
  "",
  "shimazu_takahisa",
  "shimazu_yoshihisa",
  "shimazu_yoshihiro",
  "shimazu_iyehisa",
  "",
  "ijuin_tadakura",
  "ijuin_tadamune",
  "satake_yoshinobu",
  "ujiie_tadamune",
  "utsunomiya_nobutsuna",
  "",
  "chou_tsugutsura",
  "yusa_tsugumitsu",
  "kojima_motoshige",
  "houjo_gennan",
  "",
  "fuuma_kotaro",
  "fuuma_kotaro",
  "sanada_nobuyuki",
  "sanada_yukimura",
  "okabe_motonobu",
  "ishin_suuden",
  "shichiri_yorichika",
  "otani_yoshitsugu",
  "kato_yoshiaki",
  "oda_urakusai",
  "maeda_genni",
  "",
  "",
  "nagano_masanari",
  "houjo_ujinao",
  "",
  "sanada_nobutsuna",
  "sanada_masayuki",
  "shimotsuma_raisho",
  "shimotsuma_rairen",
  "shimotsuma_rairyu",
  "",
  "kawai_yoshimune",
  "takenaka_hambe",
  "maeda_toshiie",
  "",
  "",
  "",
  "",
  "",
  "hosokawa_fujitaka",
  "tsutsui_jokei",
  "suzuki_shigetomo",
  "",
  "",
  "kuroda_kambe",
  "",
  "",
  "miyoshi_yoshitsugu",
  "tachibana_muneshige",
  "takahashi_joun",
  "shimazu_iehisa",
  "tanegashima_tokitaka",
  "",
  "",
  "",
  "watanabe_tsuna",
  "honda_tadakatsu",
  "",
  "akechi_hidemitsu",
  "ikeda_terumasa",
  "furuta_oribe",
  "kyogoku_takatsugu",
  "momochi_tamba",
  "",
  "yagyu_sogen",
  "yagyu_munenori",
  "takayam_ukon",
  "kuroda_nagamasa",
  "goto_matabe",
  "sasaki_kojiro",
  "",
  "konishi_yukinaga",
  "miyamoto_musashi",
  "kobayakawa_hidekane",
  "kobayakawa_hideaki",
  "sogo_nagayasu",
  "",
  "hashiba_hideyoshi",
  "imai_sokyu",
  "sennno_rikyu",
  "uesugi_kagekatsu",
  "",
  "kakizaki_kageie",
  "usami_sadamitsu",
  "",
  "saito_tomonobu",
  "kitajo_takahiro",
  "",
  "satomi_yoshiaki",
  "satomi_yoshihiro",
  "",
  "",
  "",
  "jimbo_nagamoto",
  "",
  "houjo_ujimasa",
  "houjo_ujiteru",
  "houjo_ujikuni",
  "houjo_ujinori",
  "houjo_tsunashige",
  "takeda_yoshinobu",
  "takeda_nobushige",
  "takeda_nobukado",
  "takeda_katsuyori",
  "",
  "",
  "yamamoto_kansuke",
  "anayama_nobukimi",
  "oyamada_nobushige",
  "imagawa_ujizane",
  "yamagata_masakage",
  "naito_masatoyo",
  "kosaka_masanobu",
  "baba_nobuharu",
  "akiyama_nobutomo",
  "sanada_masayuki",
  "asahina_yasutomo",
  "",
  "anegakoji_yoritsuna",
  "anegakoji_nobutsuna",
  "sakai_tadatsugu",
  "ishikawa_kazumasa",
  "tokugawa_nobuyasu",
  "tokugawa_hideyasu",
  "tokugawa_hidetada",
  "sakakibara_yasumasa",
  "torii_mototada",
  "hattori_hanzo",
  "honganji_kouju",
  "",
  "kidukuri_norimasa",
  "kuki_yoshitaka",
  "asakura_yoshikage",
  "",
  "",
  "saito_yoshitatsu",
  "saito_tatsuoki",
  "inaba_ittetsu",
  "ando_morishige",
  "ujiie_bokuzen",
  "oda_nobutada",
  "oda_nobukatsu",
  "oda_nobutaka",
  "shibata_katsuie",
  "niwa_nagahide",
  "sakuma_nobumori",
  "takigawa_kazumasu",
  "mori_rammaru",
  "hachisuka_koroku",
  "ikeda_tsuneoki",
  "sassa_masanari",
  "azai_nagamasa",
  "todo_takatora",
  "ishida_mitsunari",
  "rokkaku_yoshiharu",
  "gamo_ujisato",
  "isshiki_yoshikiyo",
  "isshiki_yoshisada",
  "ashikaga_yoshiteru",
  "ashikaga_yoshiaki",
  "hosokawa_tadaoki",
  "wada_koremasa",
  "matsunaga_hisahide",
  "tsutsui_junkei",
  "suzuki_sadayu",
  "suzuki_shigehide",
  "",
  "miyoshi_nagahaya",
  "miyoshi_masayasu",
  "",
  "",
  "",
  "",
  "",
  "",
  "ukita_hideie",
  "",
  "",
  "amago_yoshihisa",
  "yamanaka_shikanosuke",
  "mouri_takamoto",
  "mouri_terumoto",
  "kobayakawa_takakage",
  "shimizu_muneharu",
  "kikkawa_motoharu",
  "kikkawa_hiroie",
  "",
  "iwanari_tomomichi",
  "",
  "",
  "",
  "chousokabe_motochika",
  "chousokabe_nobuchika",
  "chousokabe_morichika",
  "kousokabe_chikayasu",
  "honjo_shigetsuna",
  "",
  "nishina_morinobu",
  "ii_naomasa",
  "oda_nobukane",
  "oda_nobuhide",
  "",
  "sakuma_morimasa",
  "",
  "asano_nagamasa",
  "yamauchi_kazutoyo",
  "kato_kiyomasa",
  "fukushima_masanori",
  "kuchiba_michiyoshi",
  "",
  "hashiba_hidenaga",
  "marume_chokei",
  "takugen",
  "",
  "louis_frois",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "kani_saizo",
  "unno_rokuro",
  "anayama_kosuke",
  "mochiduki_rokuro",
  "nedu_jimpachi",
  "kakei_juzo",
  "miyoshi_masakatsu",
  "hattori_saizo",
  "kouduki_sasuke",
  "yuri_kamanosuke",
  "toyotomi_hideyori",
  "sanada_dausuke",
};

void nb4cut_output(FILE *fin, uint32_t palette[], int id)
{
  FILE *fout;
  char foutname[BUFSIZ];

  sprintf( foutname, "face%03d%s.bmp", id, facename[id] );
  if( !( fout = nb4cut_open( foutname, "w" ) ) )
    exit( EXIT_FAILURE );
  nb4cut_write_header( fout );
  nb4cut_write_palette( fout, palette );
  nb4cut_write_pixel( fin, fout, id );
  fclose( fout );
}

#define FINNAME  "Facedata.nb4"

int main(int argc, char *argv[])
{
  FILE *fin;
  uint32_t palette[NB4_NCOLOR];
  int id;

  if( !( fin = nb4cut_open( argc > 1 ? argv[1] : FINNAME, "r" ) ) )
    exit( EXIT_FAILURE );
  nb4cut_read_palette( fin, palette );
  for( id=0; id<NB4_NFACE; id++ )
    nb4cut_output( fin, palette, id );
  fclose( fin );
  return EXIT_SUCCESS;
}