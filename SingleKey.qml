import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

Item {
    id: _key_row

    property alias _1: _key1.text
    signal click_1(QtObject _1)

    property alias _2: _key2.text
    signal click_2(QtObject _2)

    property alias _3: _key3.text
    signal click_3(QtObject _3)

    property alias _4: _key4.text
    signal click_4(QtObject _4)

    property alias _5: _key5.text
    signal click_5(QtObject _5)

    property alias _6: _key6.text
    signal click_6(QtObject _6)

    property alias _7: _key7.text
    signal click_7(QtObject _7)

    property alias _8: _key8.text
    signal click_8(QtObject _8)

    property alias _9: _key9.text
    signal click_9(QtObject _9)

    property alias _0: _key0.text
    signal click_0(QtObject _0)

    property alias _q: _keyq.text
    signal click_q(QtObject _q)

    property alias _w: _keyw.text
    signal click_w(QtObject _w)

    property alias _e: _keye.text
    signal click_e(QtObject _e)

    property alias _r: _keyr.text
    signal click_r(QtObject _r)

    property alias _t: _keyt.text
    signal click_t(QtObject _t)

    property alias _z: _keyz.text
    signal click_z(QtObject _z)

    property alias _u: _keyu.text
    signal click_u(QtObject _u)

    property alias _i: _keyi.text
    signal click_i(QtObject _i)

    property alias _o: _keyo.text
    signal click_o(QtObject _o)

    property alias _p: _keyp.text
    signal click_p(QtObject _p)

    property alias _a: _keya.text
    signal click_a(QtObject _a)

    property alias _s: _keys.text
    signal click_s(QtObject _s)

    property alias _d: _keyd.text
    signal click_d(QtObject _d)

    property alias _f: _keyf.text
    signal click_f(QtObject _f)

    property alias _g: _keyg.text
    signal click_g(QtObject _g)

    property alias _h: _keyh.text
    signal click_h(QtObject _h)

    property alias _j: _keyj.text
    signal click_j(QtObject _j)

    property alias _k: _keyk.text
    signal click_k(QtObject _k)

    property alias _l: _keyl.text
    signal click_l(QtObject _l)

    property alias _y: _keyy.text
    signal click_y(QtObject _y)

    property alias _x: _keyx.text
    signal click_x(QtObject _x)

    property alias _c: _keyc.text
    signal click_c(QtObject _c)

    property alias _v: _keyv.text
    signal click_v(QtObject _v)

    property alias _b: _keyb.text
    signal click_b(QtObject _b)

    property alias _n: _keyn.text
    signal click_n(QtObject _n)

    property alias _m: _keym.text
    signal click_m(QtObject _m)



    //property alias _Del: _keyDel.text
    //signal click_Del(QtObject _Del)

    //Row{
    GridLayout{
        id: _gridlayout
        columns: 10
        columnSpacing: 3

        MoTalk_Key{
            id: _key1
            text: '1'
            //Layout.preferredWidth: 2
            //Layout.fillWidth: true
            //Layout.fillHeight: true
            onClicked: _key_row.click_1(_key_row._1)
        }
        MoTalk_Key{
            id: _key2
            text: '2'
            onClicked: _key_row.click_2(_key_row._2)
        }

        MoTalk_Key{
            id: _key3
            text: '3'
            onClicked: _key_row.click_3(_key_row._3)
        }
        MoTalk_Key{
            id: _key4
            text: '4'
            onClicked: _key_row.click_4(_key_row._4)
        }

        MoTalk_Key{
            id: _key5
            text: '5'
            onClicked: _key_row.click_5(_key_row._5)
        }

        MoTalk_Key{
            id: _key6
            text: '6'
            onClicked: _key_row.click_6(_key_row._6)
        }
        MoTalk_Key{
            id: _key7
            text: '7'
            onClicked: _key_row.click_7(_key_row._7)
        }

        MoTalk_Key{
            id: _key8
            text: '8'
            onClicked: _key_row.click_8(_key_row._8)
        }
        MoTalk_Key{
            id: _key9
            text: '9'
            onClicked: _key_row.click_9(_key_row._9)
        }

        MoTalk_Key{
            id: _key0
            text: '0'
            onClicked: _key_row.click_0(_key_row._0)
        }

        //new line
        MoTalk_Key{
            id: _keyq
            text: 'q'
            onClicked: _key_row.click_q(_key_row._q)
        }
        MoTalk_Key{
            id: _keyw
            text: 'w'
            onClicked: _key_row.click_w(_key_row._w)
        }

        MoTalk_Key{
            id: _keye
            text: 'e'
            onClicked: _key_row.click_e(_key_row._e)
        }

        MoTalk_Key{
            id: _keyr
            text: 'r'
            onClicked: _key_row.click_r(_key_row._r)
        }

        MoTalk_Key{
            id: _keyt
            text: 't'
            onClicked: _key_row.click_t(_key_row._t)
        }

        MoTalk_Key{
            id: _keyz
            text: 'z'
            onClicked: _key_row.click_z(_key_row._z)
        }

        MoTalk_Key{
            id: _keyu
            text: 'u'
            onClicked: _key_row.click_u(_key_row._u)
        }

        MoTalk_Key{
            id: _keyi
            text: 'i'
            onClicked: _key_row.click_i(_key_row._i)
        }

        MoTalk_Key{
            id: _keyo
            text: 'o'
            onClicked: _key_row.click_o(_key_row._o)
        }

        MoTalk_Key{
            id: _keyp
            text: 'p'
            onClicked: _key_row.click_p(_key_row._p)
        }

        MoTalk_Key{
            id: _keya
            text: 'a'
            onClicked: _key_row.click_a(_key_row._a)
        }

        MoTalk_Key{
            id: _keys
            text: 's'
            onClicked: _key_row.click_s(_key_row._s)
        }

        MoTalk_Key{
            id: _keyd
            text: 'd'
            onClicked: _key_row.click_d(_key_row._d)
        }

        MoTalk_Key{
            id: _keyf
            text: 'f'
            onClicked: _key_row.click_f(_key_row._f)
        }

        MoTalk_Key{
            id: _keyg
            text: 'g'
            onClicked: _key_row.click_g(_key_row._g)
        }

        MoTalk_Key{
            id: _keyh
            text: 'h'
            onClicked: _key_row.click_h(_key_row._h)
        }

        MoTalk_Key{
            id: _keyj
            text: 'j'
            onClicked: _key_row.click_j(_key_row._j)
        }

        MoTalk_Key{
            id: _keyk
            text: 'k'
            onClicked: _key_row.click_k(_key_row._k)
        }

        MoTalk_Key{
            id: _keyl
            text: 'l'
            onClicked: _key_row.click_l(_key_row._l)
        }

        MoTalk_Key{
            id: _keyy
            text: 'y'
            onClicked: _key_row.click_y(_key_row._y)
        }

        MoTalk_Key{
            id: _keyx
            text: 'x'
            onClicked: _key_row.click_x(_key_row._x)
        }

        MoTalk_Key{
            id: _keyc
            text: 'c'
            onClicked: _key_row.click_c(_key_row._c)
        }

        MoTalk_Key{
            id: _keyv
            text: 'v'
            onClicked: _key_row.click_v(_key_row._v)
        }

        MoTalk_Key{
            id: _keyb
            text: 'b'
            onClicked: _key_row.click_b(_key_row._b)
        }

        MoTalk_Key{
            id: _keyn
            text: 'n'
            onClicked: _key_row.click_n(_key_row._n)
        }

        MoTalk_Key{
            id: _keym
            text: 'm'
            onClicked: _key_row.click_m(_key_row._m)
        }
        //new line
        MoTalk_Key{
            id: _keyDel
            text: 'Del'
            onClicked: _textfield.remove(0,20)
        }

    }


}
