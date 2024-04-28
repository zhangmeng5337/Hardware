/**
 ****************************************************************************************************
 * @file        pymb.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-22
 * @brief       ƴ�����뷨 ����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 2021-11-22
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __PY_MB_h
#define __PY_MB_h

#include "./T9INPUT/pyinput.h"


/* �������б� */
const char PY_mb_space []={""};
const char PY_mb_a     []={"������߹﹅����H����"};
const char PY_mb_ai    []={"�������������������������������������������"};
const char PY_mb_an    []={"�����������������ϰ����������������"};
const char PY_mb_ang   []={"�������a"};
const char PY_mb_ao    []={"�������������°ð���������������������������"};
const char PY_mb_ba    []={"�˰ͰְΰŰưǰȰɰʰ̰ϰаѰҰӰ԰���������������"};
const char PY_mb_bai   []={"�װذٰڰ۰ܰݰ޲�����"};
const char PY_mb_ban   []={"���߰������������������������"};
const char PY_mb_bang  []={"���������������������"};
const char PY_mb_bao   []={"�����������������������������������������������������"};
const char PY_mb_bei   []={"���������������������������������������������������������"};;
const char PY_mb_ben   []={"���������������"};
const char PY_mb_beng  []={"�������±ñı�ܡ���"};
const char PY_mb_bi    []={"�ƱǱȱɱʱ˱̱ͱαϱбѱұӱԱ�ذ�ֱױرٱڱ۱ܱ�������������ݩ��޵�������������������������������������������"};
const char PY_mb_bian  []={"�߱�����ޱ�������������������������������"};
const char PY_mb_biao  []={"�����������������������"};
const char PY_mb_bie   []={"�������"};
const char PY_mb_bin   []={"������������������������������"};
const char PY_mb_bing  []={"������������������������������"};
const char PY_mb_bo    []={"�ذٰ�����������������������������������������������ެ�������������������"};
const char PY_mb_bu    []={"��������������������������߲�����������"};
const char PY_mb_ca    []={"������"};
const char PY_mb_cai   []={"�ŲƲǲ²òĲȲɲʲ˲�"};
const char PY_mb_can   []={"�ͲβϲвѲҲ����������"};
const char PY_mb_cang  []={"�Բղֲײ���"};
const char PY_mb_cao   []={"�ݲٲڲ۲�ܳ�������"};
const char PY_mb_ce    []={"����޲���"};
const char PY_mb_cen   []={"����"};
const char PY_mb_ceng  []={"�������"};
const char PY_mb_cha   []={"������������ɲ���������������������"};
const char PY_mb_chai  []={"�����٭�������"};
const char PY_mb_chan  []={"���������������������������������������������"};
const char PY_mb_chang []={"�������������������������������������������������"};
const char PY_mb_chao  []={"�������������������½�������"};
const char PY_mb_che   []={"�����������������"};
const char PY_mb_chen  []={"���������������³óĳ����������������"};
const char PY_mb_cheng []={"�ųƳǳȳɳʳ˳̳ͳγϳгѳҳ�ʢة�����������������������"};
const char PY_mb_chi   []={"�Գճֳ׳سٳڳ۳ܳݳ޳߳������߳������ܯ����������������������������"};
const char PY_mb_chong []={"����������������������"};
const char PY_mb_chou  []={"�������������ٱ������"};
const char PY_mb_chu   []={"����������������������������������ءۻ�������������������"};
const char PY_mb_chuai []={"����������"};
const char PY_mb_chuan []={"������������������������"};
const char PY_mb_chuang[]={"����������������"};
const char PY_mb_chui  []={"��������������"};
const char PY_mb_chun  []={"��������������ݻ����"};
const char PY_mb_chuo  []={"������������"};
const char PY_mb_ci    []={"�˴̴ʹ��Ų�ôĴŴƴǴȴɴ���������������"};
const char PY_mb_cong  []={"�ӴԴϴдѴ������������"};
const char PY_mb_cou   []={"������"};
const char PY_mb_cu    []={"�ִ״ش��������������"};
const char PY_mb_cuan  []={"�ڴ۴�����ߥ����"};
const char PY_mb_cui   []={"�ݴ޴ߴ����������������"};
const char PY_mb_cun   []={"��������"};
const char PY_mb_cuo   []={"��������������������������"};
const char PY_mb_da    []={"��������������������������"};
const char PY_mb_dai   []={"���������������������������ܤ߰߾��������"};
const char PY_mb_dan   []={"��������������������������������ʯ��������������������"};
const char PY_mb_dang  []={"����������������������"};
const char PY_mb_dao   []={"��������������������������߶���������"};
const char PY_mb_de    []={"�µõĵ��"};
const char PY_mb_dei   []={"��"};
const char PY_mb_deng  []={"�εŵƵǵȵɵʵ������������"};
const char PY_mb_di    []={"�ĵֵ׵صٵڵ۵ܵݵ̵͵εϵеѵҵӵԵյ���ص��ڮ��ۡ��ݶ��������������������"};
const char PY_mb_dia   []={"��"};
const char PY_mb_dian  []={"����������ߵ����������������������"};
const char PY_mb_diao  []={"���������������������"};
const char PY_mb_die   []={"����������������ܦ�����������"};
const char PY_mb_ding  []={"������������������������������������"};
const char PY_mb_diu   []={"����"};
const char PY_mb_dong  []={"��������������������������������"};
const char PY_mb_dou   []={"����������������������"};
const char PY_mb_du    []={"�Ƕȶɶ������������¶öĶŶƶ�ܶ�������������"};
const char PY_mb_duan  []={"�˶̶Ͷζ϶������"};
const char PY_mb_dui   []={"�ѶҶӶ�����"};
const char PY_mb_dun   []={"�ܶնֶ׶ضٶڶ۶��������������"};
const char PY_mb_duo   []={"���ȶ޶߶�����������������������"};
const char PY_mb_e     []={"������������������Ŷج������ݭ��������������������"};
const char PY_mb_ei    []={"��"};
const char PY_mb_en    []={"������"};
const char PY_mb_er    []={"�������������������������"};
const char PY_mb_fa    []={"��������������������"};
const char PY_mb_fan   []={"����������������������������������ެ����������"};
const char PY_mb_fang  []={"���������������·÷ķ�������������"};
const char PY_mb_fei   []={"�ƷǷȷɷʷ˷̷ͷηϷз�����������������������������������"};
const char PY_mb_fen   []={"�ַҷܷݷ޷߷�ӷԷշ׷طٷڷ��������������"};
const char PY_mb_feng  []={"����������������ٺۺ�������"};
const char PY_mb_fo    []={"��"};
const char PY_mb_fou   []={"���"};
const char PY_mb_fu    []={"���𸵸�����������������������������������������������������������������������������������������ۮܽ������ݳ����߻����������������������������������������������"};
const char PY_mb_ga    []={"���¼п�٤���������"};
const char PY_mb_gai   []={"�øĸŸƸǸȽ�ؤ���������"};
const char PY_mb_gan   []={"�ɸʸ˸̸͸θϸиѸҸ�������ߦ���������������"};
const char PY_mb_gang  []={"�Ըոָ׸ظٸڸ۸ܿ�����"};
const char PY_mb_gao   []={"��޸߸�ݸ�����غھ۬޻��������"};
const char PY_mb_ge    []={"�����Ǹ����������������������Ͽ�ت������ܪ��������������"};
const char PY_mb_gei   []={"��"};
const char PY_mb_gen   []={"����بݢ����"};
const char PY_mb_geng  []={"�����������������������"};
const char PY_mb_gong  []={"���������������������������������������"};
const char PY_mb_gou   []={"������������������ڸ��������������������"};
const char PY_mb_gu    []={"�ù����¹��������ĹŹƹǹȹɹʹ˹̹ͼ���ڬ��������������������������������������"};
const char PY_mb_gua   []={"�ιϹйѹҹ���ڴ�������"};
const char PY_mb_guai  []={"�Թչ���"};
const char PY_mb_guan  []={"�عٹڹ۹ܹݹ޹߹������ݸ������������"};
const char PY_mb_guang []={"������������"};
const char PY_mb_gui   []={"������������������Ȳ���������������������"};
const char PY_mb_gun   []={"�������������"};
const char PY_mb_guo   []={"������������������������������������"};
const char PY_mb_ha    []={"������"};
const char PY_mb_hai   []={"���Ⱥ�������������������"};
const char PY_mb_han   []={"��������������������������������������������������������"};
const char PY_mb_hang  []={"�к��������������"};
const char PY_mb_hao   []={"�ºúĺ������尸亾��������޶�������"};
const char PY_mb_he    []={"�ǺȺɺʺ˺̺ͺκϺӺкѺҺԺպغֺ���ڭ�������������������"};
const char PY_mb_hei   []={"�ں�"};
const char PY_mb_hen   []={"�ۺܺݺ�"};
const char PY_mb_heng  []={"�ߺ����޿����"};
const char PY_mb_hong  []={"��������������ڧݦޮް����"};
const char PY_mb_hou   []={"��������ܩ��������������"};
const char PY_mb_hu    []={"���������˺��������������������������������������������������������������������������"};
const char PY_mb_hua   []={"�������������������������"};
const char PY_mb_huai  []={"������������"};
const char PY_mb_huan  []={"�����������������������»���佻�ۨۼ����ߧ���������������"};
const char PY_mb_huang []={"�λĻŻƻǻȻɻʻ˻̻ͻϻл��������������������"};
const char PY_mb_hui   []={"��ػٻڻһӻԻջֻ׻ۻܻݻ޻߻����������ڶ����ޥ��������������������"};
const char PY_mb_hun   []={"�������ڻ��������"};
const char PY_mb_huo   []={"�ͻ��������������޽߫������������"};
const char PY_mb_ji    []={"�ƼǸ��������������������������������������ȼɼʼ����������������������������������������������¼üļż˼̼ͽ���ϵآؽ����٥����ܸ����ݽ��ު��ߴ�����������������������������������������������������������"};
const char PY_mb_jia   []={"�ҼӼμмѼԼռּ׼ؼټڼۼܼݼ޼�����٤ۣ��������������������������������"};
const char PY_mb_jian  []={"�������򽡼��߼����������������������������������������������������������������������������������������������������"};
const char PY_mb_jiang []={"���罩����������������������ǿ����������������"};
const char PY_mb_jiao  []={"�нϾ���У�������������������������½ýĽŽƽȽɽʽ˽̽ͽν�ٮ��ܴ����������������������"};
const char PY_mb_jie   []={"������ҽܽӽԽ���ݽ޽սֽ׽ؽٽڽ۽߽�������ڦڵ�������������������������"};
const char PY_mb_jin   []={"�����������������������������������ݣ������������������������"};
const char PY_mb_jing  []={"������������������������������������������������������������ݼ������������������"};
const char PY_mb_jiong []={"����������"};
const char PY_mb_jiu   []={"�ž;Ⱦ��������¾þľƾɾʾ˾̾ξ�����������������"};
const char PY_mb_ju    []={"�޾߾���峵���ӾԾվ־׾ؾپھ۾ܾݾ���ϾоѾ���ڪ��������������������������������������������"};
const char PY_mb_juan  []={"��������Ȧ۲�������������"};
const char PY_mb_jue   []={"���������Ǿ����ž������������ܥާ���������������������"};
const char PY_mb_jun   []={"��������������������������������"};
const char PY_mb_ka    []={"��������������"};
const char PY_mb_kai   []={"�������������������������"};
const char PY_mb_kan   []={"������������٩�����ݨ�ۼ��"};
const char PY_mb_kang  []={"��������������������"};
const char PY_mb_kao   []={"����������������"};
const char PY_mb_ke    []={"�ɿʿ˿ƿ̿Ϳ�྿����¿ÿĿſǿ���������������������������������"};
const char PY_mb_ken   []={"�Ͽпѿ���"};
const char PY_mb_keng  []={"�ӿ������"};
const char PY_mb_kong  []={"�տֿ׿�������"};
const char PY_mb_kou   []={"�ٿڿۿ���ޢߵ����"};
const char PY_mb_ku    []={"�ݿ޿߿���������ܥ�"};
const char PY_mb_kua   []={"������٨�"};
const char PY_mb_kuai  []={"��������ۦ������"};
const char PY_mb_kuan  []={"�����"};
const char PY_mb_kuang []={"�����������ڲڿ������������"};
const char PY_mb_kui   []={"����������������������ظ���������������������������"};
const char PY_mb_kun   []={"��������������������"};
const char PY_mb_kuo   []={"����������"};
const char PY_mb_la    []={"�������������������������"};
const char PY_mb_lai   []={"���������������������"};
const char PY_mb_lan   []={"����������������������������������������"};
const char PY_mb_lang  []={"������������������������ݹ"};
const char PY_mb_lao   []={"�������������������������������������"};
const char PY_mb_le    []={"��������߷����"};
const char PY_mb_lei   []={"��������������������������ڳ������������"};
const char PY_mb_leng  []={"������ܨ�"};
const char PY_mb_li    []={"��������������������������������������������������������������������تٳٵ۪����ݰ��޼߿�����������������������������������������������������������"};
const char PY_mb_lia   []={"��"};
const char PY_mb_lian  []={"������������������������������������������������"};
const char PY_mb_liang []={"������������������������ܮݹ�����"};
const char PY_mb_liao  []={"��������������������������ޤ������������"};
const char PY_mb_lie   []={"�������������������������"};
const char PY_mb_lin   []={"�����������������������������������������������"};
const char PY_mb_ling  []={"��������������������������������۹��������������������"};
const char PY_mb_liu   []={"����������������������µ½����������������"};
const char PY_mb_lo    []={"��"}; 
const char PY_mb_long  []={"��Ū��������¡¢£¤���������������"};
const char PY_mb_lou   []={"¥¦§¨©ª¶�����������������"};
const char PY_mb_lu    []={"��«¬­®¯°±²³´µ¶·¸¹º»¼½¾����ߣ������������������������������������"};
const char PY_mb_lv    []={"��������������������������¿�������������"}; 
const char PY_mb_luan  []={"������������"};
const char PY_mb_lue   []={"�����"};
const char PY_mb_lun   []={"����������������"};
const char PY_mb_luo   []={"���޸����������������������������������������������������"};
const char PY_mb_m     []={"߼"};  
const char PY_mb_ma    []={"������������������ĦĨ��������"};
const char PY_mb_mai   []={"������������������áܬ��������������"};
const char PY_mb_man   []={"����������á������"};
const char PY_mb_mang  []={"âãäåæç��������"};
const char PY_mb_mao   []={"èéêëì��îïðñòó��í�����������������"};
const char PY_mb_me    []={"ô"};
const char PY_mb_mei   []={"ÿ����õö��������÷øùúûüýþ��ݮ�����������"};
const char PY_mb_men   []={"�������������"};
const char PY_mb_meng  []={"��������������������ޫ��������������"};
const char PY_mb_mi    []={"����������������������������������ڢ������������������������"};
const char PY_mb_mian  []={"�����������������������������"};
const char PY_mb_miao  []={"������������������������������"};
const char PY_mb_mie   []={"����ؿ�����"};
const char PY_mb_min   []={"������������������������������"};
const char PY_mb_ming  []={"������������ڤ���������"};
const char PY_mb_miu   []={"����"};
const char PY_mb_mo    []={"��û��ġĢģĤĥĦħĨĩĪīĬĭĮįİ���������������������������"};
const char PY_mb_mou   []={"ıĲĳٰ����������"};
const char PY_mb_mu    []={"ľĿ��ģĲĴĵĶķĸĹĺĻļĽ���������������"};
const char PY_mb_na    []={"����������������������"};
const char PY_mb_nai   []={"����������ؾ٦ܵ����"};
const char PY_mb_nan   []={"����������������"};
const char PY_mb_nang  []={"������߭��"};
const char PY_mb_nao   []={"����������ث������������"};
const char PY_mb_ne    []={"����ګ"};
const char PY_mb_nei   []={"����"};
const char PY_mb_nen   []={"���"};
const char PY_mb_neng  []={"��"};
const char PY_mb_ng    []={"��"}; 
const char PY_mb_ni    []={"������������������������٣�����������������"};
const char PY_mb_nian  []={"��������������إ���������"};
const char PY_mb_niang []={"����"};
const char PY_mb_niao  []={"������������"};
const char PY_mb_nie   []={"�������������������ؿ�����"};
const char PY_mb_nin   []={"��"};
const char PY_mb_ning  []={"��������šŢ�������"};
const char PY_mb_niu   []={"ţŤťŦ�������"};
const char PY_mb_nong  []={"ŧŨũŪٯ��"};
const char PY_mb_nou   []={"��"};
const char PY_mb_nu    []={"ūŬŭ��������"};
const char PY_mb_nuan  []={"ů"};
const char PY_mb_nue   []={"űŰ"};
const char PY_mb_nuo   []={"��ŲųŴŵ�������"};
const char PY_mb_nv    []={"Ů�����"};
const char PY_mb_o     []={"Ŷ���"};
const char PY_mb_ou    []={"ŷŸŹźŻżŽ��ک�����"};
const char PY_mb_pa    []={"�ǰ�žſ��������������"};
const char PY_mb_pai   []={"��������������ٽ����"};
const char PY_mb_pan   []={"�������������������������������������"};
const char PY_mb_pang  []={"����������������������"};
const char PY_mb_pao   []={"������������������������"};
const char PY_mb_pei   []={"�����������������������������"};
const char PY_mb_pen   []={"������"};
const char PY_mb_peng  []={"������������������������������ܡ���"};
const char PY_mb_pi    []={"�ٷ�����������������ơƢƣƤƥƦƧاƨƩ�����������ۯ����ܱ��ߨ�����������������������"};
const char PY_mb_pian  []={"���ƪƫƬƭ��������������"};
const char PY_mb_piao  []={"ƮƯưƱ����������������"};
const char PY_mb_pie   []={"ƲƳد���"};
const char PY_mb_pin   []={"ƴƵƶƷƸ�������"};
const char PY_mb_ping  []={"��ƹƺƻƼƽƾƿ����ٷ�����"};
const char PY_mb_po    []={"��������������������������۶�����������"};
const char PY_mb_pou   []={"������"};
const char PY_mb_pu    []={"�ձ������������������������������������������������"};
const char PY_mb_qi    []={"����������������������������������������������������������������������������������ؽ�����������ٹ��ܻ��������������ޭ����������������������"};
const char PY_mb_qia   []={"����ǡǢ������"};
const char PY_mb_qian  []={"ǣǤǥǦǧǨǩǪǫǬǭǮǯǰǱǲǳǴǵǶǷǸ��ٻ����ܷ����ݡ������������������������"};
const char PY_mb_qiang []={"ǹǺǻǼǽǾǿ�������������������������"};
const char PY_mb_qiao  []={"��������������������������������ȸڽ��������������������"};
const char PY_mb_qie   []={"����������٤�����ۧ������"};
const char PY_mb_qin   []={"��������������������������������������������"};
const char PY_mb_qing  []={"���������������������������������������������������"};
const char PY_mb_qiong []={"�����������������"};
const char PY_mb_qiu   []={"�������������������ٴ������������������������"};
const char PY_mb_qu    []={"����������������ȡȢȣȤȥ������ڰ۾����ޡ޾����������������������"};
const char PY_mb_quan  []={"ȦȧȨȩȪȫȬȭȮȯȰڹ�����������������"};
const char PY_mb_que   []={"ȱȲȳȴȵȶȷȸ�����"};
const char PY_mb_qui   []={"��"};  
const char PY_mb_qun   []={"ȹȺ����"};
const char PY_mb_ran   []={"ȻȼȽȾ������"};
const char PY_mb_rang  []={"ȿ�����������"};
const char PY_mb_rao   []={"�����������"};
const char PY_mb_re    []={"������"};
const char PY_mb_ren   []={"�����������������������������������"};
const char PY_mb_reng  []={"����"};
const char PY_mb_ri    []={"��"};
const char PY_mb_rong  []={"������������������������������"};
const char PY_mb_rou   []={"������������"};
const char PY_mb_ru    []={"���������������������������޸�������"};
const char PY_mb_ruan  []={"������"};
const char PY_mb_rui   []={"�����������ި�"};
const char PY_mb_run   []={"����"};
const char PY_mb_ruo   []={"����ټ��"};
const char PY_mb_sa    []={"������ئ�����"};
const char PY_mb_sai   []={"����������"};
const char PY_mb_san   []={"����ɡɢ�����"};
const char PY_mb_sang  []={"ɣɤɥ�����"};
const char PY_mb_sao   []={"ɦɧɨɩ����ܣ����"};
const char PY_mb_se    []={"��ɪɫɬ����"};
const char PY_mb_sen   []={"ɭ"};
const char PY_mb_seng  []={"ɮ"};
const char PY_mb_sha   []={"ɯɰɱɲɳɴɵɶɷɼ��������������"};
const char PY_mb_shai  []={"ɫɸɹ"};
const char PY_mb_shan  []={"����ɺɻɼɽɾɿ��������������������դڨ�����������۷���������������������"};
const char PY_mb_shang []={"�������������������������"};
const char PY_mb_shao  []={"������������������������ۿ�����������"};
const char PY_mb_she   []={"�����������������������������������������"};
const char PY_mb_shei  []={"˭"}; 
const char PY_mb_shen  []={"ʲ����������������������������������ڷ��ݷ�������������"};
const char PY_mb_sheng []={"��������������ʡʢʣʤʥ����������"};
const char PY_mb_shi   []={"���³�ʦʮʯʧʨʩʪʫʬʭʰʱʲʳʴʵʶʷʸʹʺʻʼʽʾʿ����������������������������������������߱��ֳ������ݪ��������������������������"};
const char PY_mb_shou  []={"���������������������������"};
const char PY_mb_shu   []={"����������������������������������������������������������������ˡٿ�������ح���������������"};
const char PY_mb_shua  []={"ˢˣ�"};
const char PY_mb_shuai []={"��ˤ˥˦˧�"};
const char PY_mb_shuan []={"˨˩����"};
const char PY_mb_shuang[]={"˪˫ˬ����"};
const char PY_mb_shui  []={"ˮ˯˰˵��"};
const char PY_mb_shun  []={"˱˲˳˴"};
const char PY_mb_shuo  []={"��˵˶˷˸����������"};
const char PY_mb_si    []={"˹˺˻˼˽˾˿����������������������ٹ���������������������������"};
const char PY_mb_song  []={"��������������������������ݿڡ"};
const char PY_mb_sou   []={"�����������������������޴"};
const char PY_mb_su    []={"�����������������������������������������"};
const char PY_mb_suan  []={"�������"};
const char PY_mb_sui   []={"����������������������"};
const char PY_mb_sun   []={"������"};
const char PY_mb_suo   []={"�����������������������ɯ��������"};
const char PY_mb_ta    []={"����������̡̢�̣̤�����������������"};
const char PY_mb_tai   []={"̧̨̥̦̩̪̫̬̭��ۢ޷����������"};
const char PY_mb_tan   []={"��̴̵̶̷̸̮̯̰̱̲̳̹̺̻̼̽̾̿��۰������������"};
const char PY_mb_tang  []={"�������������������������������������������������"};
const char PY_mb_tao   []={"����������������������߶��������ػ"};
const char PY_mb_te    []={"��߯���"};
const char PY_mb_teng  []={"����������"};
const char PY_mb_ti    []={"���������������������������������������������"};
const char PY_mb_tian  []={"����������������������������"};
const char PY_mb_tiao  []={"������������٬������������������"};
const char PY_mb_tie   []={"�����������"};
const char PY_mb_ting  []={"������ͣͤͥͦͧ͢͡��������������"};
const char PY_mb_tong  []={"ͩͫͬͭͮͯͰͱͲͳʹ١��������ͪ���������"};
const char PY_mb_tou   []={"͵Ͷͷ͸������"};
const char PY_mb_tu    []={"͹ͺͻͼͽ;Ϳ��������ܢݱ������"};
const char PY_mb_tuan  []={"���������"};
const char PY_mb_tui   []={"��������������"};
const char PY_mb_tun   []={"�����������������"};
const char PY_mb_tuo   []={"˵������������������������ر٢����������������������"};
const char PY_mb_wa    []={"�������������������"};
const char PY_mb_wai   []={"������"};
const char PY_mb_wan   []={"��������������������������������������ܹݸ��������������"};
const char PY_mb_wang  []={"�����������������������������"};
const char PY_mb_wei   []={"Ϊλν��Ρ΢ΣΤάΥΦΧΨΩΫέήίΰαβγδεζηθικμξο������������ޱ�����������������������������������"};
const char PY_mb_wen   []={"�������������������������������"};
const char PY_mb_weng  []={"��������޳"};
const char PY_mb_wo    []={"���������������������ݫ��������"};
const char PY_mb_wu    []={"�����������������������أ����������������������������������������������������������������������������������"};
const char PY_mb_xi    []={"ϣϰϦϷϸ����������������������ϡ��ϢϤϥϧϨϪϫϬϮϯϱϲϴϵ϶���������������ϭϩϳ��������ۭݾ���������������������������������������������"};
const char PY_mb_xia   []={"ϹϺϻϼϽϾϿ���������������������������"};
const char PY_mb_xian  []={"������������������ϳϴ��������������������������������������ݲ޺������������������������������"};
const char PY_mb_xiang []={"������������������������������������������ܼ������������"};
const char PY_mb_xiao  []={"С��Ф��������������������ТУХЦЧ���������������������"};
const char PY_mb_xie   []={"ЩЪЫЬЭЮЯабвгдежзийклмѪ��ШҶ��������ޯߢ���������������������"};
const char PY_mb_xin   []={"ноп��������������ܰݷ����ض����"};
const char PY_mb_xing  []={"��ʡ�����������������������������������ߩ���"};
const char PY_mb_xiong []={"��������������ܺ"};
const char PY_mb_xiu   []={"������������������������������������"};
const char PY_mb_xu    []={"��������������������������������������ڼ������ޣ������������������"};
const char PY_mb_xuan  []={"��ѡ��������������Ѥ��ѣѢ����������������������ȯ"};
const char PY_mb_xue   []={"��ѥѦѧѨѩѪ���������"};
const char PY_mb_xun   []={"ѰѮѸѶѯѵѫѬѭѱѲѳѴѷۨ����޹������࿣ݡަ����������"};
const char PY_mb_ya    []={"��ѿ����ѹ��ѺѻѼѽѾ����������������������������������"};
const char PY_mb_yan   []={"��������������������������������������������������������������������������������ٲ��Ǧ����������ڥ����۱۳ܾ�����������������������������"};
const char PY_mb_yang  []={"������������������������������������������������"};
const char PY_mb_yao   []={"Ҫ��ҧҩ����ҡҢ���ңҤҥҦҨҫԿزس��ߺ����ű�����������������������"};
const char PY_mb_ye    []={"ҲҳҵҶҹҺҬҭҮүҰұҴҷҸа��������������������"};
const char PY_mb_yi    []={"һ��������������������ҼҽҾҿ�������������������������������������������������������������������������β��������٫ڱ������ܲ����޲������߽߮�����������������������������������������������������������������"};
const char PY_mb_yin   []={"����ӡ����������������������������ط�۴����ܧ������������������"};
const char PY_mb_ying  []={"ӳӰӢӨөӪӫӣӤӥӦӧӬӭӮӯӱӲ��ݺ�����������۫�������������������"};
const char PY_mb_yo    []={"Ӵ���"};
const char PY_mb_yong  []={"��ӵӶ��ӿӷӸӹӺӻӼӽӾ����ٸ��ܭ�����������"};
const char PY_mb_you   []={"��������������������������������������������������������٧ݬݯݵ��������������������"};
const char PY_mb_yu    []={"������������������ԡ������������������������������������������������������������ԢԣԤԥԦ��ع�����εξخ��ٶ������������������������������������������������������������������������"};
const char PY_mb_yuan  []={"ԭԪ԰ԱԲԴԵԶԸԹԺԧԨԩԫԬԮԯԳԷ��ܫܾ������������������"};
const char PY_mb_yue   []={"��������˵ԻԼԽԾԿ����������������"};
const char PY_mb_yun   []={"��Ա�������������������������ܿ��۩����������"};
const char PY_mb_za    []={"��������զ����"};
const char PY_mb_zai   []={"������������������"};
const char PY_mb_zan   []={"���������������������"};
const char PY_mb_zang  []={"�����������"};
const char PY_mb_zao   []={"������������������������������"};
const char PY_mb_ze    []={"��������զ�����������������"};
const char PY_mb_zei   []={"��"};
const char PY_mb_zen   []={"����"};
const char PY_mb_zeng  []={"��������������"};
const char PY_mb_zha   []={"��բ����������ագդեզէըթ߸��������������������"};
const char PY_mb_zhai  []={"լխծ����ժիկ���"};
const char PY_mb_zhan  []={"ռսվչմ��հձղճյնշոպ��ջտ�������"};
const char PY_mb_zhang []={"�������������������������˳������������۵���"};
const char PY_mb_zhao  []={"����צ���׳�����گ������������������"};
const char PY_mb_zhe   []={"������������������������ߡ��������������"};
const char PY_mb_zhen  []={"��������������������������������֡����������������������������"};
const char PY_mb_zheng []={"��֤��������������������֢֣ں����������"};
const char PY_mb_zhi   []={"ְֻʶֱֲֳִֵ��ֽ֧־ַָ������ֹ��ֶֺּֿ֥֦֪֭֮֫֬֯��������������֨֩������������ش��ۤ������������������������������������������������������������"};
const char PY_mb_zhong []={"����������������������ڣ���������"};
const char PY_mb_zhou  []={"����������������������������ݧ����������������"};
const char PY_mb_zhu   []={"��סעף������������������������������������������פ������٪����ۥ��������������������������������ؼ"};
const char PY_mb_zhua  []={"ץצ��"};
const char PY_mb_zhuai []={"קת"};
const char PY_mb_zhuan []={"��רשת׫׬׭�������"};
const char PY_mb_zhuang[]={"��׮ׯװױײ׳״���"};
const char PY_mb_zhui  []={"׵׶׷׸׹׺�������"};
const char PY_mb_zhun  []={"׻׼����"};
const char PY_mb_zhuo  []={"��׽׾׿��������������پ��ߪ�����������"};
const char PY_mb_zi    []={"���������������������������������֨�������������������������������������"};
const char PY_mb_zong  []={"����������������������"};
const char PY_mb_zou   []={"������������۸����"};
const char PY_mb_zu    []={"����������������������"};
const char PY_mb_zuan  []={"��߬������"};
const char PY_mb_zui   []={"��������ީ��"};
const char PY_mb_zun   []={"��������ߤ"};
const char PY_mb_zuo   []={"�����������������������������������"};

//ƴ��������
const py_index py_index3[]=
{
{"" ,"",(char*)PY_mb_space},
{"2","a",(char*)PY_mb_a},
{"3","e",(char*)PY_mb_e},
{"6","o",(char*)PY_mb_o},
{"24","ai",(char*)PY_mb_ai},
{"26","an",(char*)PY_mb_an},
{"26","ao",(char*)PY_mb_ao},
{"22","ba",(char*)PY_mb_ba},
{"24","bi",(char*)PY_mb_bi},
{"26","bo",(char*)PY_mb_bo},
{"28","bu",(char*)PY_mb_bu},
{"22","ca",(char*)PY_mb_ca},
{"23","ce",(char*)PY_mb_ce},
{"24","ci",(char*)PY_mb_ci},
{"28","cu",(char*)PY_mb_cu},
{"32","da",(char*)PY_mb_da},
{"33","de",(char*)PY_mb_de},
{"34","di",(char*)PY_mb_di},
{"38","du",(char*)PY_mb_du},
{"36","en",(char*)PY_mb_en},
{"37","er",(char*)PY_mb_er},
{"32","fa",(char*)PY_mb_fa},
{"36","fo",(char*)PY_mb_fo},
{"38","fu",(char*)PY_mb_fu},
{"42","ha",(char*)PY_mb_ha},
{"42","ga",(char*)PY_mb_ga},
{"43","ge",(char*)PY_mb_ge},
{"43","he",(char*)PY_mb_he},
{"48","gu",(char*)PY_mb_gu},
{"48","hu",(char*)PY_mb_hu},
{"54","ji",(char*)PY_mb_ji},
{"58","ju",(char*)PY_mb_ju},
{"52","ka",(char*)PY_mb_ka},
{"53","ke",(char*)PY_mb_ke},
{"58","ku",(char*)PY_mb_ku},
{"52","la",(char*)PY_mb_la},
{"53","le",(char*)PY_mb_le},
{"54","li",(char*)PY_mb_li},
{"58","lu",(char*)PY_mb_lu},
{"58","lv",(char*)PY_mb_lv},
{"62","ma",(char*)PY_mb_ma},
{"63","me",(char*)PY_mb_me},
{"64","mi",(char*)PY_mb_mi},
{"66","mo",(char*)PY_mb_mo},
{"68","mu",(char*)PY_mb_mu},
{"62","na",(char*)PY_mb_na},
{"63","ne",(char*)PY_mb_ne},
{"64","ni",(char*)PY_mb_ni},
{"68","nu",(char*)PY_mb_nu},
{"68","nv",(char*)PY_mb_nv},
{"68","ou",(char*)PY_mb_ou},
{"72","pa",(char*)PY_mb_pa},
{"74","pi",(char*)PY_mb_pi},
{"76","po",(char*)PY_mb_po},
{"78","pu",(char*)PY_mb_pu},
{"74","qi",(char*)PY_mb_qi},
{"78","qu",(char*)PY_mb_qu},
{"73","re",(char*)PY_mb_re},
{"74","ri",(char*)PY_mb_ri},
{"78","ru",(char*)PY_mb_ru},
{"72","sa",(char*)PY_mb_sa},
{"73","se",(char*)PY_mb_se},
{"74","si",(char*)PY_mb_si},
{"78","su",(char*)PY_mb_su},
{"82","ta",(char*)PY_mb_ta},
{"83","te",(char*)PY_mb_te},
{"84","ti",(char*)PY_mb_ti},
{"88","tu",(char*)PY_mb_tu},
{"92","wa",(char*)PY_mb_wa},
{"96","wo",(char*)PY_mb_wo},
{"98","wu",(char*)PY_mb_wu},
{"94","xi",(char*)PY_mb_xi},
{"98","xu",(char*)PY_mb_xu},
{"92","ya",(char*)PY_mb_ya},
{"93","ye",(char*)PY_mb_ye},
{"94","yi",(char*)PY_mb_yi},
{"96","yo",(char*)PY_mb_yo},
{"98","yu",(char*)PY_mb_yu},
{"92","za",(char*)PY_mb_za},
{"93","ze",(char*)PY_mb_ze},
{"94","zi",(char*)PY_mb_zi},
{"98","zu",(char*)PY_mb_zu},
{"264","ang",(char*)PY_mb_ang},
{"224","bai",(char*)PY_mb_bai},
{"226","ban",(char*)PY_mb_ban},
{"226","bao",(char*)PY_mb_bao},
{"234","bei",(char*)PY_mb_bei},
{"236","ben",(char*)PY_mb_ben},
{"243","bie",(char*)PY_mb_bie},
{"246","bin",(char*)PY_mb_bin},
{"224","cai",(char*)PY_mb_cai},
{"226","can",(char*)PY_mb_can},
{"226","cao",(char*)PY_mb_cao},
{"242","cha",(char*)PY_mb_cha},
{"243","che",(char*)PY_mb_che},
{"244","chi",(char*)PY_mb_chi},
{"248","chu",(char*)PY_mb_chu},
{"268","cou",(char*)PY_mb_cou},
{"284","cui",(char*)PY_mb_cui},
{"286","cun",(char*)PY_mb_cun},
{"286","cuo",(char*)PY_mb_cuo},
{"324","dai",(char*)PY_mb_dai},
{"326","dan",(char*)PY_mb_dan},
{"326","dao",(char*)PY_mb_dao},
{"334","dei",(char*)PY_mb_dei},
{"343","die",(char*)PY_mb_die},
{"348","diu",(char*)PY_mb_diu},
{"368","dou",(char*)PY_mb_dou},
{"384","dui",(char*)PY_mb_dui},
{"386","dun",(char*)PY_mb_dun},
{"386","duo",(char*)PY_mb_duo},
{"326","fan",(char*)PY_mb_fan},
{"334","fei",(char*)PY_mb_fei},
{"336","fen",(char*)PY_mb_fen},
{"368","fou",(char*)PY_mb_fou},
{"424","gai",(char*)PY_mb_gai},
{"426","gan",(char*)PY_mb_gan},
{"426","gao",(char*)PY_mb_gao},
{"434","gei",(char*)PY_mb_gei},
{"436","gen",(char*)PY_mb_gen},
{"468","gou",(char*)PY_mb_gou},
{"482","gua",(char*)PY_mb_gua},
{"484","gui",(char*)PY_mb_gui},
{"486","gun",(char*)PY_mb_gun},
{"486","guo",(char*)PY_mb_guo},
{"424","hai",(char*)PY_mb_hai},
{"426","han",(char*)PY_mb_han},
{"426","hao",(char*)PY_mb_hao},
{"434","hei",(char*)PY_mb_hei},
{"436","hen",(char*)PY_mb_hen},
{"468","hou",(char*)PY_mb_hou},
{"482","hua",(char*)PY_mb_hua},
{"484","hui",(char*)PY_mb_hui},
{"486","hun",(char*)PY_mb_hun},
{"486","huo",(char*)PY_mb_huo},
{"542","jia",(char*)PY_mb_jia},
{"543","jie",(char*)PY_mb_jie},
{"546","jin",(char*)PY_mb_jin},
{"548","jiu",(char*)PY_mb_jiu},
{"583","jue",(char*)PY_mb_jue},
{"586","jun",(char*)PY_mb_jun},
{"524","kai",(char*)PY_mb_kai},
{"526","kan",(char*)PY_mb_kan},
{"526","kao",(char*)PY_mb_kao},
{"536","ken",(char*)PY_mb_ken},
{"568","kou",(char*)PY_mb_kou},
{"582","kua",(char*)PY_mb_kua},
{"584","kui",(char*)PY_mb_kui},
{"586","kun",(char*)PY_mb_kun},
{"586","kuo",(char*)PY_mb_kuo},
{"524","lai",(char*)PY_mb_lai},
{"526","lan",(char*)PY_mb_lan},
{"526","lao",(char*)PY_mb_lao},
{"534","lei",(char*)PY_mb_lei},
{"543","lie",(char*)PY_mb_lie},
{"546","lin",(char*)PY_mb_lin},
{"548","liu",(char*)PY_mb_liu},
{"568","lou",(char*)PY_mb_lou},
{"583","lue",(char*)PY_mb_lue},
{"586","lun",(char*)PY_mb_lun},
{"586","luo",(char*)PY_mb_luo},
{"624","mai",(char*)PY_mb_mai},
{"626","man",(char*)PY_mb_man},
{"626","mao",(char*)PY_mb_mao},
{"634","mei",(char*)PY_mb_mei},
{"636","men",(char*)PY_mb_men},
{"643","mie",(char*)PY_mb_mie},
{"646","min",(char*)PY_mb_min},
{"648","miu",(char*)PY_mb_miu},
{"668","mou",(char*)PY_mb_mou},
{"624","nai",(char*)PY_mb_nai},
{"626","nan",(char*)PY_mb_nan},
{"626","nao",(char*)PY_mb_nao},
{"634","nei",(char*)PY_mb_nei},
{"636","nen",(char*)PY_mb_nen},
{"643","nie",(char*)PY_mb_nie},
{"646","nin",(char*)PY_mb_nin},
{"648","niu",(char*)PY_mb_niu},
{"683","nue",(char*)PY_mb_nue},
{"686","nuo",(char*)PY_mb_nuo},
{"724","pai",(char*)PY_mb_pai},
{"726","pan",(char*)PY_mb_pan},
{"726","pao",(char*)PY_mb_pao},
{"734","pei",(char*)PY_mb_pei},
{"736","pen",(char*)PY_mb_pen},
{"743","pie",(char*)PY_mb_pie},
{"746","pin",(char*)PY_mb_pin},
{"768","pou",(char*)PY_mb_pou},
{"742","qia",(char*)PY_mb_qia},
{"743","qie",(char*)PY_mb_qie},
{"746","qin",(char*)PY_mb_qin},
{"748","qiu",(char*)PY_mb_qiu},
{"783","que",(char*)PY_mb_que},
{"786","qun",(char*)PY_mb_qun},
{"726","ran",(char*)PY_mb_ran},
{"726","rao",(char*)PY_mb_rao},
{"736","ren",(char*)PY_mb_ren},
{"768","rou",(char*)PY_mb_rou},
{"784","rui",(char*)PY_mb_rui},
{"786","run",(char*)PY_mb_run},
{"786","ruo",(char*)PY_mb_ruo},
{"724","sai",(char*)PY_mb_sai},
{"726","sao",(char*)PY_mb_sao},
{"726","san",(char*)PY_mb_san},
{"736","sen",(char*)PY_mb_sen},
{"742","sha",(char*)PY_mb_sha},
{"743","she",(char*)PY_mb_she},
{"744","shi",(char*)PY_mb_shi},
{"748","shu",(char*)PY_mb_shu},
{"768","sou",(char*)PY_mb_sou},
{"784","sui",(char*)PY_mb_sui},
{"786","sun",(char*)PY_mb_sun},
{"786","suo",(char*)PY_mb_suo},
{"824","tai",(char*)PY_mb_tai},
{"826","tan",(char*)PY_mb_tan},
{"826","tao",(char*)PY_mb_tao},
{"843","tie",(char*)PY_mb_tie},
{"868","tou",(char*)PY_mb_tou},
{"884","tui",(char*)PY_mb_tui},
{"886","tun",(char*)PY_mb_tun},
{"886","tuo",(char*)PY_mb_tuo},
{"924","wai",(char*)PY_mb_wai},
{"926","wan",(char*)PY_mb_wan},
{"934","wei",(char*)PY_mb_wei},
{"936","wen",(char*)PY_mb_wen},
{"942","xia",(char*)PY_mb_xia},
{"943","xie",(char*)PY_mb_xie},
{"946","xin",(char*)PY_mb_xin},
{"948","xiu",(char*)PY_mb_xiu},
{"983","xue",(char*)PY_mb_xue},
{"986","xun",(char*)PY_mb_xun},
{"926","yan",(char*)PY_mb_yan},
{"926","yao",(char*)PY_mb_yao},
{"946","yin",(char*)PY_mb_yin},
{"968","you",(char*)PY_mb_you},
{"983","yue",(char*)PY_mb_yue},
{"986","yun",(char*)PY_mb_yun},
{"924","zai",(char*)PY_mb_zai},
{"926","zan",(char*)PY_mb_zan},
{"926","zao",(char*)PY_mb_zao},
{"934","zei",(char*)PY_mb_zei},
{"936","zen",(char*)PY_mb_zen},
{"942","zha",(char*)PY_mb_zha},
{"943","zhe",(char*)PY_mb_zhe},
{"944","zhi",(char*)PY_mb_zhi},
{"948","zhu",(char*)PY_mb_zhu},
{"968","zou",(char*)PY_mb_zou},
{"984","zui",(char*)PY_mb_zui},
{"986","zun",(char*)PY_mb_zun},
{"986","zuo",(char*)PY_mb_zuo},
{"2264","bang",(char*)PY_mb_bang},
{"2364","beng",(char*)PY_mb_beng},
{"2426","bian",(char*)PY_mb_bian},
{"2426","biao",(char*)PY_mb_biao},
{"2464","bing",(char*)PY_mb_bing},
{"2264","cang",(char*)PY_mb_cang},
{"2364","ceng",(char*)PY_mb_ceng},
{"2424","chai",(char*)PY_mb_chai},
{"2426","chan",(char*)PY_mb_chan},
{"2426","chao",(char*)PY_mb_chao},
{"2436","chen",(char*)PY_mb_chen},
{"2468","chou",(char*)PY_mb_chou},
{"2484","chuai",(char*)PY_mb_chuai},
{"2484","chui",(char*)PY_mb_chui},
{"2486","chun",(char*)PY_mb_chun},
{"2486","chuo",(char*)PY_mb_chuo},
{"2664","cong",(char*)PY_mb_cong},
{"2826","cuan",(char*)PY_mb_cuan},
{"3264","dang",(char*)PY_mb_dang},
{"3364","deng",(char*)PY_mb_deng},
{"3426","dian",(char*)PY_mb_dian},
{"3426","diao",(char*)PY_mb_diao},
{"3464","ding",(char*)PY_mb_ding},
{"3664","dong",(char*)PY_mb_dong},
{"3826","duan",(char*)PY_mb_duan},
{"3264","fang",(char*)PY_mb_fang},
{"3364","feng",(char*)PY_mb_feng},
{"4264","gang",(char*)PY_mb_gang},
{"4364","geng",(char*)PY_mb_geng},
{"4664","gong",(char*)PY_mb_gong},
{"4824","guai",(char*)PY_mb_guai},
{"4826","guan",(char*)PY_mb_guan},
{"4264","hang",(char*)PY_mb_hang},
{"4364","heng",(char*)PY_mb_heng},
{"4664","hong",(char*)PY_mb_hong},
{"4823","huai",(char*)PY_mb_huai},
{"4826","huan",(char*)PY_mb_huan},
{"5426","jian",(char*)PY_mb_jian},
{"5426","jiao",(char*)PY_mb_jiao},
{"5464","jing",(char*)PY_mb_jing},
{"5826","juan",(char*)PY_mb_juan},
{"5264","kang",(char*)PY_mb_kang},
{"5364","keng",(char*)PY_mb_keng},
{"5664","kong",(char*)PY_mb_kong},
{"5824","kuai",(char*)PY_mb_kuai},
{"5826","kuan",(char*)PY_mb_kuan},
{"5264","lang",(char*)PY_mb_lang},
{"5366","leng",(char*)PY_mb_leng},
{"5426","lian",(char*)PY_mb_lian},
{"5426","liao",(char*)PY_mb_liao},
{"5464","ling",(char*)PY_mb_ling},
{"5664","long",(char*)PY_mb_long},
{"5826","luan",(char*)PY_mb_luan},
{"6264","mang",(char*)PY_mb_mang},
{"6364","meng",(char*)PY_mb_meng},
{"6426","mian",(char*)PY_mb_mian},
{"6426","miao",(char*)PY_mb_miao},
{"6464","ming",(char*)PY_mb_ming},
{"6264","nang",(char*)PY_mb_nang},
{"6364","neng",(char*)PY_mb_neng},
{"6426","nian",(char*)PY_mb_nian},
{"6426","niao",(char*)PY_mb_niao},
{"6464","ning",(char*)PY_mb_ning},
{"6664","nong",(char*)PY_mb_nong},
{"6826","nuan",(char*)PY_mb_nuan},
{"7264","pang",(char*)PY_mb_pang},
{"7364","peng",(char*)PY_mb_peng},
{"7426","pian",(char*)PY_mb_pian},
{"7426","piao",(char*)PY_mb_piao},
{"7464","ping",(char*)PY_mb_ping},
{"7426","qian",(char*)PY_mb_qian},
{"7426","qiao",(char*)PY_mb_qiao},
{"7464","qing",(char*)PY_mb_qing},
{"7826","quan",(char*)PY_mb_quan},
{"7264","rang",(char*)PY_mb_rang},
{"7364","reng",(char*)PY_mb_reng},
{"7664","rong",(char*)PY_mb_rong},
{"7826","ruan",(char*)PY_mb_ruan},
{"7264","sang",(char*)PY_mb_sang},
{"7364","seng",(char*)PY_mb_seng},
{"7424","shai",(char*)PY_mb_shai},
{"7426","shan",(char*)PY_mb_shan},
{"7426","shao",(char*)PY_mb_shao},
{"7434","shei",(char*)PY_mb_shei},
{"7436","shen",(char*)PY_mb_shen},    
{"7468","shou",(char*)PY_mb_shou},
{"7482","shua",(char*)PY_mb_shua},
{"7484","shui",(char*)PY_mb_shui},
{"7486","shun",(char*)PY_mb_shun},
{"7486","shuo",(char*)PY_mb_shuo},
{"7664","song",(char*)PY_mb_song},
{"7826","suan",(char*)PY_mb_suan},
{"8264","tang",(char*)PY_mb_tang},
{"8364","teng",(char*)PY_mb_teng},
{"8426","tian",(char*)PY_mb_tian},
{"8426","tiao",(char*)PY_mb_tiao},
{"8464","ting",(char*)PY_mb_ting},
{"8664","tong",(char*)PY_mb_tong},
{"8826","tuan",(char*)PY_mb_tuan},
{"9264","wang",(char*)PY_mb_wang},
{"9364","weng",(char*)PY_mb_weng},
{"9426","xian",(char*)PY_mb_xian},
{"9426","xiao",(char*)PY_mb_xiao},
{"9464","xing",(char*)PY_mb_xing},
{"9826","xuan",(char*)PY_mb_xuan},
{"9264","yang",(char*)PY_mb_yang},
{"9464","ying",(char*)PY_mb_ying},
{"9664","yong",(char*)PY_mb_yong},
{"9826","yuan",(char*)PY_mb_yuan},
{"9264","zang",(char*)PY_mb_zang},
{"9364","zeng",(char*)PY_mb_zeng},
{"9424","zhai",(char*)PY_mb_zhai},
{"9426","zhan",(char*)PY_mb_zhan},
{"9426","zhao",(char*)PY_mb_zhao},
{"9436","zhen",(char*)PY_mb_zhen},
{"9468","zhou",(char*)PY_mb_zhou},
{"9482","zhua",(char*)PY_mb_zhua},
{"9484","zhui",(char*)PY_mb_zhui},
{"9486","zhun",(char*)PY_mb_zhun},
{"9486","zhuo",(char*)PY_mb_zhuo},
{"9664","zong",(char*)PY_mb_zong},
{"9826","zuan",(char*)PY_mb_zuan},
{"24264","chang",(char*)PY_mb_chang},
{"24364","cheng",(char*)PY_mb_cheng},
{"24664","chong",(char*)PY_mb_chong},
{"24826","chuan",(char*)PY_mb_chuan},
{"48264","guang",(char*)PY_mb_guang},
{"48264","huang",(char*)PY_mb_huang},
{"54264","jiang",(char*)PY_mb_jiang},
{"54664","jiong",(char*)PY_mb_jiong},
{"58264","kuang",(char*)PY_mb_kuang},
{"54264","liang",(char*)PY_mb_liang},
{"64264","niang",(char*)PY_mb_niang},
{"74264","qiang",(char*)PY_mb_qiang},
{"74664","qiong",(char*)PY_mb_qiong},
{"74264","shang ",(char*)PY_mb_shang},
{"74364","sheng",(char*)PY_mb_sheng},
{"74824","shuai",(char*)PY_mb_shuai},
{"74826","shuan",(char*)PY_mb_shuan},
{"94264","xiang",(char*)PY_mb_xiang},
{"94664","xiong",(char*)PY_mb_xiong},
{"94264","zhang",(char*)PY_mb_zhang},
{"94364","zheng",(char*)PY_mb_zheng},
{"94664","zhong",(char*)PY_mb_zhong},
{"94824","zhuai",(char*)PY_mb_zhuai},
{"94826","zhuan",(char*)PY_mb_zhuan},
{"248264","chuang",(char*)PY_mb_chuang},
{"748264","shuang",(char*)PY_mb_shuang},
{"948264","zhuang",(char*)PY_mb_zhuang},
};

#endif











