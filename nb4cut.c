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
#define NB4_NMON          256 /* number of montages */
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

void nb4cut_write_pixel(FILE *fin, FILE *fout, int id, int nface)
{
  int32_t val;
  int i, j;

  if( id >= nface ){
    fprintf( stderr, "invalid face identifier %d/%d\n", id, nface );
    return;
  }
  fseek( fin, NB4_HEADERSIZE + NB4_NCOLOR*4 + ( nface - id - 1 ) * NB4_WIDTH * NB4_HEIGHT * NB4_BPP, SEEK_SET );
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

const char *nb4facename[] = {
  "伊達晴宗",
  "伊達輝宗",
  "伊達政宗",
  "伊達成実",
  "片倉景綱",
  "蘆名盛氏",
  "最上義守",
  "最上義光",
  "上杉謙信",
  "上杉景虎",
  "直江兼続",
  "佐竹義昭",
  "佐竹義重",
  "宇都宮広綱",
  "結城政勝",
  "結城晴朝",
  "上杉憲政",
  "上杉憲賢",
  "北条氏康",
  "武田信玄",
  "今川義元",
  "太原雪斎",
  "飯富虎昌",
  "朝比奈泰能",
  "徳川家康",
  "本願寺光佐",
  "朝倉宗滴",
  "明智光秀",
  "斎藤道三",
  "織田信長",
  "織田信行",
  "林通勝",
  "森可成",
  "浅井久政",
  "磯野員昌",
  "六角義賢",
  "一色義幸",
  "島左近",
  "荒木村重",
  "波多野晴通",
  "毛利元就",
  "安国寺恵瓊",
  "大内義長",
  "陶晴賢",
  "長宗我部国親",
  "大友宗麟",
  "大友義統",
  "立花道雪",
  "龍造寺隆信",
  "龍造寺政家",
  "鍋島直茂",
  "相良晴広",
  "相良義陽",
  "伊東義祐",
  "島津貴久",
  "島津義久",
  "島津義弘",
  "島津家久",
  "伊集院忠朗",
  "伊集院忠倉",
  "伊集院忠棟",
  "佐竹義宣",
  "氏家忠棟",
  "宇都宮国綱",
  "足利義氏",
  "長続連",
  "遊佐続光",
  "小島職鎮",
  "北条幻庵",
  "松田憲秀",
  "風魔小太郎",
  "風魔小太郎",
  "真田信之",
  "真田幸村",
  "岡部元信",
  "以心崇伝",
  "七里頼周",
  "大谷吉継",
  "加藤嘉明",
  "織田有楽斎",
  "前田玄以",
  "長尾政景",
  "里見義康",
  "長野業正",
  "北条氏直",
  "鵜殿長照",
  "真田信綱",
  "真田昌幸",
  "下間頼照",
  "下間頼廉",
  "下間頼竜",
  "九鬼守隆",
  "河合吉統",
  "竹中半兵衛",
  "前田利家",
  "赤尾清綱",
  "雨森弥兵衛",
  "海北綱親",
  "蒲生賢秀",
  "京極高吉",
  "細川藤孝",
  "筒井順慶",
  "鈴木重朝",
  "",
  "赤井直正",
  "黒田官兵衛",
  "",
  "立原久綱",
  "三好義継",
  "立花宗茂",
  "高橋紹運",
  "島津家久",
  "種子島時尭",
  "蘆名盛隆",
  "蘆名盛興",
  "酒井家次",
  "渡辺守綱",
  "本多忠勝",
  "大久保忠世",
  "明智秀満",
  "池田輝政",
  "古田織部",
  "京極高次",
  "百地丹波",
  "筒井定次",
  "柳生宗厳",
  "柳生宗矩",
  "高山右近",
  "黒田長政",
  "後藤又兵衛",
  "佐々木小次郎",
  "尼子経久",
  "小西行長",
  "宮本武蔵",
  "小早川秀包",
  "小早川秀秋",
  "十河存保",
  "三好長治",
  "羽柴秀吉",
  "今井宗久",
  "千利休",
  "上杉景勝",
  "上杉景信",
  "柿崎景家",
  "宇佐美定満",
  "直江景綱",
  "斎藤朝信",
  "北条高広",
  "上条政繁",
  "里見義尭",
  "里見義弘",
  "里見義頼",
  "畠山義綱",
  "畠山義慶",
  "神保長職",
  "神保長住",
  "北条氏政",
  "北条氏照",
  "北条氏邦",
  "北条氏規",
  "北条綱成",
  "武田義信",
  "武田信繁",
  "武田信廉",
  "武田勝頼",
  "武田信豊",
  "",
  "山本勘助",
  "穴山信君",
  "小山田信繁",
  "今川氏真",
  "山県昌景",
  "内藤昌豊",
  "高坂昌信",
  "馬場信房",
  "秋山信友",
  "真田幸隆",
  "朝比奈泰朝",
  "姉小路良頼",
  "姉小路頼綱",
  "姉小路信綱",
  "酒井忠次",
  "石川数正",
  "徳川信康",
  "徳川秀康",
  "徳川秀忠",
  "榊原康政",
  "鳥居元忠",
  "服部半蔵",
  "本願寺光寿",
  "北畠具教",
  "木造具政",
  "九鬼嘉隆",
  "朝倉義景",
  "朝倉景健",
  "朝倉景鏡",
  "斎藤義竜",
  "斎藤龍興",
  "稲葉一鉄",
  "安藤守就",
  "氏家卜全",
  "織田信忠",
  "織田信雄",
  "織田信孝",
  "柴田勝家",
  "丹羽長秀",
  "佐久間信盛",
  "滝川一益",
  "森蘭丸",
  "蜂須賀小六",
  "池田恒興",
  "佐々政成",
  "浅井長政",
  "藤堂高虎",
  "石田三成",
  "六角義治",
  "蒲生氏郷",
  "一色義道",
  "一色義定",
  "足利義輝",
  "足利義昭",
  "細川忠興",
  "和田惟政",
  "松永久秀",
  "筒井順慶",
  "鈴木佐大夫",
  "鈴木重秀",
  "三好長慶",
  "三好長逸",
  "三好政康",
  "波多野秀治",
  "赤松義祐",
  "赤松則房",
  "山名豊国",
  "山名祐豊",
  "宇喜多直家",
  "宇喜多秀家",
  "宇喜多忠家",
  "尼子晴久",
  "尼子義久",
  "山中鹿介",
  "毛利隆元",
  "毛利輝元",
  "小早川隆景",
  "清水宗春",
  "吉川元春",
  "吉川広家",
  "十河一存",
  "岩成友通",
  "三好義賢",
  "河野通宣",
  "河野通直",
  "長宗我部元親",
  "長宗我部信親",
  "長曽我部盛親",
  "香宗我部親泰",
  "本庄繁長",
  "矢沢頼貞",
  "仁科盛信",
  "井伊直政",
  "織田信包",
  "織田信秀",
  "堀秀政",
  "佐久間盛政",
  "金森長近",
  "浅野長政",
  "山内一豊",
  "加藤清正",
  "福島正則",
  "口羽通良",
  "",
  "羽柴秀長",
  "丸目長恵",
  "沢彦",
  "近衛前久",
  "ルイス・フロイス",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "姫",
  "",
  "矢沢頼綱",
  "可児才蔵",
  "海野六郎",
  "穴山小助",
  "望月六郎",
  "根津甚八",
  "筧十蔵",
  "三好政勝",
  "服部才蔵",
  "上月佐助",
  "由利鎌之介",
  "豊臣秀頼",
  "真田大助",
};

void nb4cut_output(FILE *fin, uint32_t palette[], int id, int nface)
{
  FILE *fout;
  char foutname[BUFSIZ];

  if( nface == NB4_NFACE )
    sprintf( foutname, "face%03d%s.bmp", id, nb4facename[id] );
  else
    sprintf( foutname, "face_mon%03d.bmp", id ); /* montages */
  if( !( fout = nb4cut_open( foutname, "w" ) ) )
    exit( EXIT_FAILURE );
  nb4cut_write_header( fout );
  nb4cut_write_palette( fout, palette );
  nb4cut_write_pixel( fin, fout, id, nface );
  fclose( fout );
}

int nb4cut_output_batch(char filename[], int nface)
{
  FILE *fin;
  uint32_t palette[NB4_NCOLOR];
  int id;

  if( !( fin = nb4cut_open( filename, "r" ) ) ) return -1;
  nb4cut_read_palette( fin, palette );
  for( id=0; id<nface; id++ )
    nb4cut_output( fin, palette, id, nface );
  fclose( fin );
  return 0;
}

#define FINNAME     "Facedata.nb4"
#define FINNAME_MON "Mondata.nb4"

int main(int argc, char *argv[])
{
  /* original face data */
  nb4cut_output_batch( argc > 1 ? argv[1] : FINNAME, NB4_NFACE );
  /* montage data */
  nb4cut_output_batch( argc > 2 ? argv[2] : FINNAME_MON, NB4_NMON );
  return EXIT_SUCCESS;
}
