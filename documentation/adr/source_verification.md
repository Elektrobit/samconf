# ADR - Verification of a configuration source

The content of a configuration source shall be verified by a signature method
to verify its integrity and origin.

## Requirements and Constraints

The verification process shall cover the following requirements and constraints:

* The verification shall be based on the content of a file
* The signature can be provided by the file or separately (still to discuss)
* The signature is always applied to the entire content of a file, no
  mixed usage of signed and unsigned content in the same configuration source
* The implementation shall allow mixed usage of signed and unsigned content for
  different configuration sources
* The signature process itself shall not be implemented by samconf, it is
  planned to delegate this via an external crypto API to some sort of dedicated
  external crypto component
* The concept shall be extensible to support encrypted configuration sources in future

## The signature

For the signature the following requirements shall be met:

* To be future-proof the signature shall contain information about the
  signature mechanism to use.
* The signature shall be in a format that can be easily attached to a file
  without causing problems about used characters

To make a signature self explanatory it shall use a URI like syntax as follows:

```
signature_method://signature_payload
sha1://f0f8f92add392aff18dedad014732fc1dfb4427e
md5://9ce83ff3647269cf3b341824e243a25f
x509://RHVtbXkgY2VydCA6KQo=
http://external.verify.myconfig.somwhere/signature/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
unix+tcp:///run/cryptoservice.sock/signature/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
file:///dev/special_crypto_device/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
```

The signature has to be stored somewhere with a reference to the corresponding
file. The following options are considered :

* A) append / prepend to a file
```
signature_method://signature_payload
{
 "option": {...},
...
}
signature_method://signature_payload
```
* B) encode in the file name
```
/etc/myservice.d/feature_c2hhMTovL2YwZjhmOTJhZGQzOTJhZmYxOGRlZGFkMDE0NzMyZmMxZGZiNDQyN2UK.conf
```
* C) in a separate file
```
/etc/myservice.d/feature.conf /etc/myservice.d/feature.sig
```
* D) in a signature DB obtained from a file or a local service
```
/etc/myservice.d/feature.conf
cat /etc/signature.db
  /etc/myservice.d/feature.conf sha1://f0f8f92add392aff18dedad014732fc1dfb4427e

curl http://localhost:44444/etc/myservice.d/feature.conf
200 OK
...
/etc/myservice.d/feature.conf sha1://f0f8f92add392aff18dedad014732fc1dfb4427e
```
* E) in a extended file attribute
 * two usable namespaces `trusted` and `user`
 * either read and write by everyone (`user`) or only root user (`trusted`)
```
# only by root modifyable 'trusted' namespace
sudo setfattr -n trusted.signature -v "sha1://f0f8f92add392aff18dedad014732fc1dfb4427e" /etc/myservice.d/feature.conf
# only by root readable 'trusted' namespace
sudo getfattr -n trusted.signature /etc/myservice.d/feature.conf
# file: /etc/myservice.d/feature.conf
trusted.signature="sha1://f0f8f92add392aff18dedad014732fc1dfb4427e"

setfattr -n user.signature -v "sha1://f0f8f92add392aff18dedad014732fc1dfb4427e" /etc/myservice.d/feature.conf
getfattr -n user.signature /etc/myservice.d/feature.conf
# file: /etc/myservice.d/feature.conf
user.signature="sha1://f0f8f92add392aff18dedad014732fc1dfb4427e"
```

| Method | pro            | con |
|--------|----------------|-----|
| A      | simple         | can confuse tools which are not aware of this additional content, i.e. xml,json parser |
| B      | simple         | names will become extremely large, which is unusual or unexpected and can cause trouble |
| C      | simple, signature size and format not limited  | noisy in the file system; need to sync the contents of two files |
| D      | central independent place, flexible and extensible, control of signature is separated | additional service API necessary, not self contained |
| E      | not visible, access can be restricted to root by using 'trusted' namespace, it reads like it is made for this | filesystem must support xattr and signature size must be < 4k depending on the filesystem even smaller, needs extra caution on coping file '-a'|

To be future proof the process of obtaining the signature string for a
configuration source shall be a implementation detail of the configuration
backend. A backend can use a common component to lookup the signature string
but is not enforced to do so.

### Decision

Variant C is used.

## how to protect against loading self signed or not intended signed configs?

The config verification process must ensure that no signed configs can be
introduced which for example are copied from a different system in debug mode.

* The signature shall somehow include or bound to the specific hardware, the
  machine aka. hardware-id is a promising candidate for this
```
signature = sign(Config + hardwareID)
```
* The process shall ensure that options from a signed source are not
  overwritten by one from an unsigned source, to prevent enabling debug mode or
  similar by just adding self signed config files. To prevent this each option
  shall have a flag `isSigned`, this flag will be set by the configuration
  backend if the option is read from a signed and successfully verified source.
  An option with a signed flag has always precedence and will overwrite any
  other option. An option with a sign flag can not be overwritten.
