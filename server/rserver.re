Netsmtp.Debug.enable := true;

Nettls_gnutls.init() |> ignore;

let timeout = 60.0;
let domain = "smtp.gmail.com";
let port = 587;
let provider = Netsys_crypto.current_tls();

let addr =
  `Socket((
    `Sock_inet_byname((Unix.SOCK_STREAM, domain, port)),
    Uq_client.default_connect_options,
  ));

let tc =
  Netsys_tls.create_x509_config(
    ~peer_auth=`None,
    ~system_trust=true,
    provider,
  );

let c = (new Netsmtp.connect)(addr, timeout);
c#helo();
c#starttls(tc, ~peer_name=Some(domain));

c#auth(
  (module Netmech_plain_sasl.PLAIN),
  Config.emailUser,
  "",
  [("password", Config.emailPass, [])],
  [],
);

/* c#auth(
     (module Netmech_digest_sasl.DIGEST_MD5),
     Config.emailUser,
     "LOGIN",
     [("password", Config.emailPass, [])],
     [("digest-uri", "smtp/smtp", true)],
   ); */
/*

 let tls = Netsys_crypto.current_tls();
 let tc = Netsys_tls.create_x509_config(~trust:[`PEM_file "/etc/ssl/certs/ca-certificates.crt" ] ~peer_auth:`None tls;;
 let c  = new Netsmtp.connect addr 300.0;;
 c#helo();;
 c#starttls tc;;
 c # auth (module Netmech_digestmd5_sasl.DIGEST_MD5) "gerd" "" [ "password", "secret", [] ] [ "digest-uri", "smtp/smtp", true];;


 Netsmtp.authenticate ~tls_config:tc ~sasl_mechs:[ (module Netmech_digestmd5_sasl.DIGEST_MD5); (module Netmech_crammd5_sasl.CRAM_MD5) ] ~user:"gerd" ~creds:["password", "secret", []] c ;;
  */