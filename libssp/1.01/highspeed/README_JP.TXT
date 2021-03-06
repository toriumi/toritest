[Start of Document]
==============================================================================
●高速撮影プログラム highspeed    簡易マニュアル

                                                      R0  2016/12/19  南風 曜

==============================================================================

●高速撮影アプリ "highspeed" をmakeする前の事前準備

SSPライブラリを予めインストールしておいてください。

●Make方法

以下の様に、SSPライブラリをインストールしたディレクトリ配下に highspeed が
展開しされていることを確認してください。

SSPライブラリが
例えば、/home/pi/libssp/1.01/
にインストールされている場合には、

/home/pi/libssp/1.01/highspeed/ としてください。
※それを前提にしてMakefileを作成しています。

  README_JP.TXT   このファイル
  README_EN.TXT   英語版README
  Makefile        makeファイル
  highspeed.c     高速撮影プログラム
  preview.c       RAWプレビュープログラム

SSPライブラリが正しくインストール出来ている場合には、LXTerminalにて、

----------------------------------------------------------------------------
$ make
----------------------------------------------------------------------------

とするだけで、実行ファイル
  highspeed
  preview

が作成されます。

●高速撮影プログラム実行方法

----------------------------------------------------------------------------
$ cd /home/pi/libssp/1.01/highspeed/
$ ./highspeed IMX219_640x480_187fps_10bit_2lane.xml 230 2000 50
----------------------------------------------------------------------------

  IMX219_640x480_187fps_10bit_2lane.xml : xmlファイル
  230                                   : アナログゲイン
  2000                                  : デジタルゲイン
  50                                    : 露光時間

  パラメータの値は一例です。

●RAWプレビュープログラム実行方法

ファイル名の部分は、例えば 640x480のRAWファイルが
/home/pi/libssp/1.01/highspeed/raw
に保存されている場合、LXTerminalにて、

----------------------------------------------------------------------------
$ cd /home/pi/libssp/1.01/highspeed/
$ ./preview 640 480 raw/frame*.raw
----------------------------------------------------------------------------

  640            : プレビューするRAWファイルの横サイズ
  480            : プレビューするRAWファイルの縦サイズ
  raw/frame*.raw : RAWファイル(ワイルドカード可)

とすることで、全てのフレームをプレビューできます。


●リアルタイムプレビューモード用コマンド
q     終了 (highspeed/preview共通)
c     簡易ホワイトバランスゲイン調整を行う (highspeed/preview共通)
r     録画 (highspeedのみ)


●プレビューモード用コマンド(highspeed/preview共通)
q      終了
r      録画
v      リアルタイムプレビュー
w      ビデオ出力(非圧縮)
W      ビデオ出力(Motion JPEG形式)
a      ビデオ出力(MP4形式)
p      ポーズ
s      スタートフレームへ
e      エンドフレームへ
z      １フレームRAW書き出し
x      全フレームRAW書き出し
t      メッセージの表示／非表示トグル
0～5   スピード調整
9      5フレームスキップして再生
8      スキップ無しで再生
[空白] 次のフレームを表示
m      前のフレームを表示
f      100フレーム先へ
b      100フレーム戻す
F      200フレーム先へ
B      200フレーム戻す

●サポート

不明点があれば、気軽に以下のWebサイトをご覧になり、
コミュニティにご意見をお寄せください。

https://www.visionproc.org/


[End of Document]
