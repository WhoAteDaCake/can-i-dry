# Sending mail via ocamlnet

```ocaml
Netsmtp.Debug.enable := true;
/* Nettls_gnutls.init() |> ignore; */

Nettls_gnutls.init() |> ignore;

let timeout = 300.0;
let domain = "smtp.gmail.com";
let port = 587;
let tls = Netsys_crypto.current_tls();

let addr =
  `Socket((
    `Sock_inet_byname((Unix.SOCK_STREAM, domain, port)),
    Uq_client.default_connect_options,
  ));

let tc =
  Netsys_tls.create_x509_config(
    ~trust=[`PEM_file("/etc/ssl/certs/ca-certificates.crt")],
    ~peer_auth=`None,
    tls,
  );
let c = (new Netsmtp.connect)(addr, timeout);
c#helo();
c#starttls(tc);
c#auth(
  (module Netmech_digest_sasl.DIGEST_MD5),
  Config.emailUser,
  "",
  [("password", Config.emailPass, [])],
  [("digest-uri", "smtp/smtp", true)],
);
```

After running it, I received a message saying: `Fatal error: exception Netsmtp.Permanent_error(530, "5.7.0 Must issue a STARTTLS command first. b74-v6sm8557341wma.8 - gsmtp")`

According to [stackoverflow](https://stackoverflow.com/questions/10509699/must-issue-a-starttls-command-first), there was something wrong with the line `c#starttls(tc);`

At this point I realised, that I haven't been reading documentation for tls properly, so I began doing that.
This has lead me to a realisation, that my `tc` had to be updated

```ocaml
let provider = Netsys_crypto.current_tls();
let tc =
  Netsys_tls.create_x509_config(
    ~peer_auth=`Required,
    ~system_trust=true,
    provider,
  );
```

Turns out i was wrong ? Changing:
```ocaml
~peer_auth=`None
```

Solves
> Will solve Fatal error: exception Nettls_gnutls.TLS.Error(NETTLS_VERIFICATION_FAILED)

However, now I get:
 > 504 5.7.4 Unrecognized Authentication Type z13sm7892196wrw.19 - gsmtp

Maybe using md5 encryption, was wrong ? Found a link on [stackoverflow](https://stackoverflow.com/questions/6787056/what-mechanism-does-gmail-use-for-user-authentication) showing what to use

However looking at `hello` call, `AUTH DIGEST-MD5` proves that i was using correct one ?

Looking at the code inside netsmtp.ml, it's probably down to this line:

```ocaml
let command =
  if !first then 
    "AUTH " ^
      Netsys_sasl.Info.mechanism_name mech ^
        ( match msg with
            | Some "" -> " ="
            | Some s -> " " ^ Netencoding.Base64.encode s
            | None -> ""
        )
```

Looking further turns out `hello` call wasn't the one returning AUTH type
Found a useful resource which talks about different returns in : http://www.ietf.org/rfc/rfc1893.txt

Managed to partially authenticate by overriding `netsmtp.ml` module and adding

```ocaml
let additional = "\u{0}" ^ Config.emailUser ^ "\u{0}" ^ Config.emailPass in
let full_command = command ^ " " ^ (Netencoding.Base64.encode additional) in
print_endline full_command;
self # smtp_cmd full_command;
```

However now
> Auth error: unexpected 235 
Is thrown. This could probably be because I've manually injected the command without passing trough the digest module