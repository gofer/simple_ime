# 「創造情報学連携講座VII」　第1回　日本語入力と形態素解析

![Alt text](https://raw.github.com/gofer/simple_ime/master/google_logo.png)

## ■氏名等
gofer (@gofer_ex) <gofer.ex.studio@gmail.com>  
  Department of Creative Informatics,  
  Graduate School of Information Science and Technology,  
  The University of Tokyo.

## ■概要
このプログラムは創造情報学連携講座VIIの第1回目の課題として作られた簡易IMEです。

## ■コンパイル方法
同梱されているcompile.shを実行するとmy_ime.outという実行可能形式ファイルが生成されます。

## ■使用法
my_ime.outを実行する前に、実行可能形式ファイルと同じディレクトリにresourceというディレクトリを作り、
その中にhttp://code.google.com/p/mozc/source/browse/#svn%2Ftrunk%2Fsrc%2Fdata%2Fdictionaryにある
dictionary[0-9].txtとconnection.txtを置きます。

次に、my_ime.outを実行すると辞書と連接コストテーブルの生成が行われるのでしばらく待ちます。（意外と時間がかかります。）

最後に、「ひらがな入力> 」というプロンプトが出たら続けて「ひらがな文字列」の入力を行い、Enterキーで確定します。
終了する場合はCtrl + Cで終了します。（メモリリークしますが、最近の賢いOSではさほど問題にならないと思います。）

## ■実行結果（抜粋）
### ○うまくいく例

* ひらがな入力> わたしはとうきょうだいがくだいがくいんのがくせいです。  
｜私｜は｜東京大学大学院｜の｜学生｜です｜。｜

* ひらがな入力> わたしはだいがくいんでけいさんきかがくのけんきゅうをしています。  
｜私｜は｜大学院｜で｜計算機｜科学｜の｜研究｜を｜してい｜ます｜。｜

* ひらがな入力> はいぱふぉーまんすこんぴゅーてぃんぐはすーぱーこんぴゅーたをもちいたけんきゅうです  
｜ハイパフォーマンスコンピューティング｜は｜スーパーコンピュータ｜を｜用いた｜研究｜です｜

* ひらがな入力> ぐーぐるににゅうしゃしたいです  
｜グーグル｜に｜入社｜し｜たい｜です｜

* ひらがな入力> おんしゃがだいいちしぼうです！  
｜御社｜が｜第一｜志望です｜！｜

* ひらがな入力> このれぽーとかだいは（ひかくてき）むずかしいぶるいにはいるとおもいます 
｜この｜レポート課題｜は｜（｜比較的｜）｜難しい｜部類｜に｜入る｜と｜思い｜ます｜

* ひらがな入力> せかいとは、おきているすべてのことである。  
｜世界｜とは｜、｜起きている｜すべて｜の｜こと｜で｜ある｜。｜

* ひらがな入力> しそうは、いぎをもつめいだである。  
｜思想｜は｜、｜意義｜を｜持つ｜めい｜だ｜で｜ある｜。｜

* ひらがな入力> しそうはいぎをもつめいだいである。  
｜思想｜は｜意義｜を｜持つ｜命題｜で｜ある｜。｜

* ひらがな入力> かたりえぬことについては、ちんもくするほかない。  
｜語りえ｜ぬ｜こと｜については｜、｜沈黙｜する｜ほかない｜。｜

### ×うまくいかない例

* ひらがな入力> 【いんようもと】うぃとげんしゅたいんちょ、のや　しげきやく　『ろんりてつがくろんこう』（ちくまがくげいぶんこ）  
｜【｜引用元｜】｜ウィトゲンシュタイン｜著｜、｜野谷｜　｜しげ｜規約｜　｜『｜論理｜哲学｜論考｜』｜（｜筑摩｜学芸文庫｜）｜

* ※あまり有名でない固有名詞は一発変換しにくい。

* ひらがな入力> くるまがくるまでまつ  
｜車｜が｜車｜で｜待つ｜

* ※同音異義語が一発変換できないことがある。  
* 　全ての分には動詞（述語）がある仮定して形態素解析の結果を用いればよいかもしれないが、
単語ごとに変換する人も少ないということと、時間的コストが高いことがデメリットである。

（以上）
