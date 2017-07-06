# Open Monero - a fully open sourced implementation of MyMonero backend

In this example [restbed](https://github.com/Corvusoft/restbed/) is used to
demonstrate how to provide Monero related JSON REST service. For this purpose, 
 a service called Open Monero was developed.


Open Monero is an open source implementation of backend of
https://mymonero.com/. The frontend, which includes HTML, CSS, JavaScript, was adapted
from (and originally developed by) https://mymonero.com/. 

Open Monero's backend is open sourced, free
to use, host and modify. Additionally, some features were added/changed as compared
to MyMonero. They include:

 - google analytics, cloudflare, images and flash were removed.
 - transaction fees were set to zero (MyMonero also has now them zero due to problem with its RingCT).
 - the wallets generated use 25 word mnemonics, fully compatible with official monero wallets
(13 word mnemonics generated by MyMonero work as usual though).
 - import wallet fee was reduced.
 - added support of testnet network and mainnet network without relying transactions
 - improved handling of mempool, coinbase, locked and unlocked transactions.
 - added dynamic miner fees.
 - minimum mixin set to 4 for the next hard fork.
 - view only mode added.
 - ability to offline show your address and private view and spend keys, based on the mnemonic seed, in case of server or connections problem resulting in not being able to login to your account.
 - private tx key given to a user which can be used to prove sending xmr to a recipient. 
 - Cross-Origin Resource Sharing (CORS) was disabled.
 - Transaction confirmation window added to double check tx details before its commited. 
 - Ability to select transaction priority. 
   
## Testnet version

- [http://139.162.32.245:81](http://139.162.32.245:81)

or alternatively, owning to [Gingeropolous](https://github.com/Gingeropolous):

- [http://testwallet.moneroworld.com](http://testwallet.moneroworld.com)

This is Open Monero running on testnet network. You can use it to play around with it. 
Since this is testnet version, frequent changes and database resets are expected. Also,
 it is running on cheap vps, which may result in some lag.

If you want some testnet monero, please make issue with your testnet address that you can
obtained from Open Monero.   
   
## Screenshot

![Open Monero](https://raw.githubusercontent.com/moneroexamples/openmonero/master/screenshot/screen1.png)


## Host it yourself

The Open Monero consists of four components that need to be setup for it to work:

 - MySql/Mariadb database - it stores user address (viewkey is not stored!), 
 associated transactions, outputs, inputs and transaction import payments information.
 - Frontend - it is virtually same as that of MyMonero, except before mentioned differences.
  It consists of HTML, CSS, and JavaScript.
 - Monero daemon - daemon must be running and fully sync, as this is 
 where all transaction data is fetched from and used. Daemon also commits txs 
 from the Open Monero into the Monero network.
 - Backend - fully written in C++. It uses [restbed](https://github.com/Corvusoft/restbed/) to serve JSON REST to the frontend 
 and [mysql++](http://www.tangentsoft.net/mysql++/) to interface the database. It also accesses Monero blockchain and "talks"
 with Monero deamon.
    
   
## Limitations

#### Performance

Open Monero is not as fast as MyMonero. This is because it is basic, easy to understand
 and straight forward implementation of the backend. Thus, it does not use any catching
 of transactions, blocks, complex database structures and SQL queries. Also, no ongoing 
 monitoring of user's transactions is happening, since viewkey is not stored. Transaction
 search threads start when user logs in (viewkey and address are submitted to the search thread), 
 and finish shorty after logout. Once the search threads stop, 
 they can't be restarted without user logging in back, because viewkey is unknown.



## Example compilation on Ubuntu 16.04 

Below are example and basic instructions on how to setup up and run Open Monero on Ubuntu 16.04. 


#### Monero download and compilation

Download and compile recent Monero realease into your home folder:

```bash
# first install monero dependecines
sudo apt update

sudo apt install git build-essential cmake libboost-all-dev miniupnpc libunbound-dev graphviz doxygen libunwind8-dev pkg-config libssl-dev libcurl4-openssl-dev libgtest-dev

# go to home folder 
cd ~

git clone https://github.com/monero-project/monero

cd monero/

# checkout last monero version
git checkout -b last_release v0.10.3.1

make
```

#### Compilation of the Open Monero (don't run it yet)

Once Monero was downloaded and compiled, we can download Open Monero and compile it. 
In fact we could postpone compilation to later, but 
we can just do it now, to see if it compiles. But don't run it yet. It will not
work without database, setup frontend, and synced and running monero blockchain.

```bash
# need mysql++ libraries 
sudo apt install libmysql++-dev
 
# go to home folder if still in ~/monero
cd ~

git clone https://github.com/moneroexamples/openmonero.git

cd openmonero

mkdir build && cd build

cmake ..

# altearnatively can use cmake -DMONERO_DIR=/path/to/monero_folder .. 
# if monero is not in ~/monero

make
```

#### Mysql/Mariadb

```bash
sudo apt install mysql-server
sudo mysql_secure_installation
```

Download `openmonero.sql` provided and setup the `openmonero` database. `openmonero.sql` script will
drop current `openmonero` if exist. So don't run it, if you have already some important information
in the `openmonero` database.

Assuming we are still in `build` folder:

```bash
# apply it to mysql
mysql -p -u root < ../sql/openmonero.sql
```

#### Lighttpd and frontend

```bash
sudo apt-get install lighttpd
```
Assuming you are still in `build` folder, copy frontend source files into lighttpd www folder.

```bash
sudo mkdir /var/www/html/openmonero
sudo cp -rvf ../html/* /var/www/html/openmonero/
```

Setup document root in `lighttpd.conf` into openmonero folder

```bash
sudo vim /etc/lighttpd/lighttpd.conf
```

and change `server.document-root` into:

```bash
server.document-root    = "/var/www/html/openmonero"
```

Restart lighttpd to see the change:

```bash
sudo systemctl restart lighttpd
```

Go to localhost (http://127.0.0.1) and check if frontend is working.


#### Run Open Monero

Command line options

```bash
./openmonero -h

  -h [ --help ] [=arg(=1)] (=0)         produce help message
  -t [ --testnet ] [=arg(=1)] (=0)      use testnet blockchain
  --do-not-relay [=arg(=1)] (=0)        does not relay txs to other nodes. 
                                        useful when testing construction and 
                                        submiting txs
  -p [ --port ] arg (=1984)             default port for restbed service of 
                                        Open Monero
  -c [ --config-file ] arg (=./config/config.json)
                                        Config file path.
```

Before running `openmonero`: 

 - make sure you have `Downloads` folder in your home directory. 
 Time library used in Open Monero stores there time zone offsets database that it uses.
 - edit `config/confing.js` file with your settings. Especially set `frontend-url` and `database`
 connection details.
 - set `apiUrl` in `html\js\config.js` and `testnet` flag. Last slash `/` in `apiUrl` is important. 
 If running backend for testnet network, frontend `testnet` flag must be set to `true`.
 For mainnet, it is set to `false`.
 - make sure monero daemon is running and fully sync. If using testnet network, use daemon
 with testnet flag!
   

To start for mainnet: 
```bash
./openmonero
```

To start for testnet: 
```bash
./openmonero -t
```
   
To start for testnet with non-default location of `config.json` file:

```bash
./openmonero -t -c /path/to/config.json
```

 
## API calls

In the below examples, you can change `127.0.0.1:1984` into `139.162.32.245:1984` to
query the testnet open monero instance (assuming its up and working well).

#### get_version


```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/get_version
```
Example output:

```json
{
  "blockchain_height": 858972,
  "last_git_commit_date": "2017-03-03",
  "last_git_commit_hash": "544bacc",
  "monero_version_full": "0.10.2.1-release"
}
```

### login

```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/login -d '{"address": "A2VTvE8bC9APsWFn3mQzgW8Xfcy2SP2CRUArD6ZtthNaWDuuvyhtBcZ8WDuYMRt1HhcnNQvpXVUavEiZ9waTbyBhP6RM8TV", "view_key": "041a241325326f9d86519b714a9b7f78b29111551757eeb6334d39c21f8b7400"}'
```

```json
{"new_address":false,"status":"success"}
```


#### get_address_txs

```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/get_address_txs -d '{"address": "A2VTvE8bC9APsWFn3mQzgW8Xfcy2SP2CRUArD6ZtthNaWDuuvyhtBcZ8WDuYMRt1HhcnNQvpXVUavEiZ9waTbyBhP6RM8TV", "view_key": "041a241325326f9d86519b714a9b7f78b29111551757eeb6334d39c21f8b7400"}'
```

Output (only part shown):

```json
{
  "blockchain_height": 858973,
  "scanned_block_height": 857881,
  "scanned_height": 0,
  "start_height": 855639,
  "total_received": 2709882623948850,
  "total_received_unlocked": 2709882623948850,
  "transactions": [
    {
      "coinbase": false,
      "hash": "19f4ca4d601db3cc518f7abf6778cc325150386ecdf116a6b124605974f05fcc",
      "height": 848286,
      "id": 2140,
      "mempool": false,
      "mixin": 4,
      "payment_id": "",
      "spent_outputs": [
        {
          "amount": 10000000000000,
          "key_image": "29c1218b5b63c3c92fd9702a3a292def8d3c94b4ea2a3a3a8b0c2dce45bbe905",
          "mixin": 4,
          "out_index": 0,
          "tx_pub_key": "8850a06305369f11a9e408739dddea7a05bdfbcd2140b0e00097a231fe37f7ac"
        },
        {
          "amount": 10000000000,
          "key_image": "9c82226bdf165fd2424d9a0ead661682bfab2fe644cd0bcef575ae16595c550c",
          "mixin": 4,
          "out_index": 0,
          "tx_pub_key": "29ceefd594b856c7c06d2423be17cd674b97d1d8f72907ace8d27f5b6aa9875c"
        }
      ],
      "timestamp": "2017-02-15 01:44:06",
      "total_received": 6974553705816,
      "total_sent": 10010000000000,
      "unlock_time": 0
    },
    {
      "coinbase": false,
      "hash": "899d41642e8439cb390737d1e38c8c06fe92f4cceb5ca9f49575ed84942453a1",
      "height": 848294,
      "id": 2142,
      "mempool": false,
      "mixin": 4,
      "payment_id": "",
      "timestamp": "2017-02-15 01:55:34",
      "total_received": 1000000000000,
      "total_sent": 0,
      "unlock_time": 0
    }
  ]
}
```

#### get_address_info

```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/get_address_info -d '{"address": "A2VTvE8bC9APsWFn3mQzgW8Xfcy2SP2CRUArD6ZtthNaWDuuvyhtBcZ8WDuYMRt1HhcnNQvpXVUavEiZ9waTbyBhP6RM8TV", "view_key": "041a241325326f9d86519b714a9b7f78b29111551757eeb6334d39c21f8b7400"}'
```

Output (only part shown):

```json
{
  "blockchain_height": 858970,
  "locked_funds": 0,
  "scanned_block_height": 858970,
  "scanned_height": 0,
  "spent_outputs": [
    {
      "amount": 13683584012406,
      "key_image": "437518836c315bf989c5cc28b935280345ed672d727122f6d6c5c5ff32e98224",
      "mixin": 0,
      "out_index": 0,
      "tx_pub_key": "99c74015a50d91d29a16f3c1e43540a0b9da858c2a09faaec167db3cc6939dbf"
    },
    {
      "amount": 13683584012406,
      "key_image": "ac3088ce17cc608bcf86db65e9061fe4b9b02573b997944e4ebf7d8e64e4a3b4",
      "mixin": 0,
      "out_index": 0,
      "tx_pub_key": "99c74015a50d91d29a16f3c1e43540a0b9da858c2a09faaec167db3cc6939dbf"
    }
  ],
  "start_height": 855633,
  "total_received": 13481878608141995,
  "total_sent": 4699871131811773
}
```


#### get_unspent_outs

```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/get_unspent_outs -d '{"address": "A2VTvE8bC9APsWFn3mQzgW8Xfcy2SP2CRUArD6ZtthNaWDuuvyhtBcZ8WDuYMRt1HhcnNQvpXVUavEiZ9waTbyBhP6RM8TV", "view_key": "041a241325326f9d86519b714a9b7f78b29111551757eeb6334d39c21f8b7400","amount":"0","mixin":4,"use_dust":false,"dust_threshold":"1000000000"}'
```

Output (only part shown):

```json
{
  "amount": 2746682935584926,
  "outputs": [
    {
      "amount": 2450000000000,
      "global_index": 86136,
      "height": 839599,
      "index": 0,
      "public_key": "6f6a4023bfa407ca1ce37f7382d5ea7540a330575bd570094b5add5e8ded2dd9",
      "rct": "4aca9e9b9a5d63fcf409ac28191696cabb78c0ba14791152509ebe6db7f9311033a1dc75d69a1dad7523f65856d07487aad2bee2098f5566b6d92ec5a5c68f00653d241a9d7f16ff13df87825609e8b2353ec20e50d11f8133d234184d9f8b03",
      "spend_key_images": [
        "2818dae0940fb945185c562fcb0a496f3c3b551f33b7ddd7ec1b5ecd856166e6"
      ],
      "timestamp": "2017-02-01 05:53:28",
      "tx_hash": "9d17084091beedc55c8a0cd342e441b7c0d89eeca25ac151b4b91fe1e12051e7",
      "tx_id": 2117,
      "tx_prefix_hash": "849f11f6b012c1557f87692ca7a67bcb24a5a553078c4faed870b8982821feee",
      "tx_pub_key": "4219b1004fa64bde0213bf3c59b9e160af8603be03d79e13148c15fd598e3a0e"
    },
    {
      "amount": 10000000000,
      "global_index": 86550,
      "height": 840011,
      "index": 0,
      "public_key": "5506e8786b7634a77487f0938a00d3de6ab005e2f76ffee05fee68d5165382f1",
      "rct": "6f78720d6bb0287d78c50bfa41332232fbc4bc9b7f04e35681ff926cc156b1fbfb3b84a2dcdc8cb98b33be70b302e380944abc3e7a7cbf3e27614936eccca40203447622e149ba1e09a7062eb8910acf029f561ecdaa5a34e076baff5d63770e",
      "spend_key_images": [
        "9c82226bdf165fd2424d9a0ead661682bfab2fe644cd0bcef575ae16595c550c"
      ],
      "timestamp": "2017-02-02 03:18:18",
      "tx_hash": "8257367ca6def69dc3d280e8909c3cd01b230fc9922c9a89217db06f3fb41102",
      "tx_id": 2118,
      "tx_prefix_hash": "781a0ce699865987ec78ad331e8e89c0d14461f9881dd11a7a4541114b567568",
      "tx_pub_key": "29ceefd594b856c7c06d2423be17cd674b97d1d8f72907ace8d27f5b6aa9875c"
    }
  ],
  "per_kb_fee": 2480631964
}
```

#### get_random_outs

```bash
curl  -w "\n" -X POST http://127.0.0.1:1984/get_random_outs -d '{"amounts":["0","0"],"count":5}'
```

Output (only part shown):

```json
  "amount_outs": [
    {
      "amount": 0,
      "outputs": [
        {
          "global_index": 48449,
          "public_key": "637dbadf193fa9fd5c50c96af18f458a9b7d4844fdf7ffdfa3f62d51d6aff835",
          "rct": "0c908e1969edfe7824560104e44334b46ced17c9462eacb5aaa70e62ea34a394837c07d14bffc5a65e2dd14da395dd135bcc2e5ac70648782d76e4a9920cd007b9ffe319b796c555fb7713a270f21181a5ee5c8b01259becba0dd332b93a6c02"
        },
        {
          "global_index": 67558,
          "public_key": "6ca3a73512dadd669430f73809c949f3edf71728bea5201441c648c2d128c453",
          "rct": "999312ca1914895cf8a517c91a54a069d8fdc7205d7768173618e77fad2fde5c725604d666b101c9ae19c72e07cf5f821603a7b63efb5dfd8a7c0e36ed0c250fa92929cda49ddd0d34e664e15634ee59e958815764ec979b5ff0a72b3af6af0a"
        },
        {
          "global_index": 102186,
          "public_key": "c29d43f5d7c71a6f1b4f3286da3c296a083cf68728d85c268ee0c964a6c8c00e",
          "rct": "23aa82efdbd0c6878060496a13f7a707a6f45649b51de12d54d0cad14c5be5bd00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        },
        {
          "global_index": 96992,
          "public_key": "fb83d74a42abe65d5b8a6a906791202376b91e3459a31737ac62a401b7b9356c",
          "rct": "975b42f50cabb801091c90a4227bf7ba024ddacfeda7e5e0383f034b0bc8ba2500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        },
        {
          "global_index": 36848,
          "public_key": "2a8785e42a9446785cf189a40fca8f56a592dab9db1f38f8e8a3d2eb84b680ff",
          "rct": "a09e27b83917792bc8f6c51b40077aea329bf08b569e63657e2ac3529db5d0aa14f9f9fc45930577b43829119847ac857d69c00d12859a903cfcbf470819dc00b1bfa43aa979e46a0d0476cb36717e8d8374afe9af1fb3715091147cc9587e0e"
        }
      ]
    }
  ]
}
```

Need to add other.


## Scrap notes (just for myself)

### Generate your own ssl certificate 
 
Setting up https and ssl certificates in restbed
 - https://github.com/Corvusoft/restbed/blob/34187502642144ab9f749ab40f5cdbd8cb17a54a/example/https_service/source/example.cpp

Based on the link above:
 
```bash
# Create Certificate
cd /tmp
openssl genrsa -out server.key 1024
openssl req -new -key server.key -out server.csr
openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt
openssl dhparam -out dh2048.pem 2048
```


## Other examples

Other examples can be found on  [github](https://github.com/moneroexamples?tab=repositories).
Please know that some of the examples/repositories are not
finished and may not work as intended.

## How can you help?

Constructive criticism, code and website edits are always good. They can be made through github.
