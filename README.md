# How it works?

## 1. The overall framework of keygen

```
             pass CID, DID, MID to
      GUI --------------------------> LIB
   -------------                -------------
            ↑                        |
            └------------------------┘
                feedback LIC to
```

### GUI Components

- **assert.h**: The ID of the resource.
- **dlgx.h, dlgx.c**: Dialog initialization, etc.
- **event.h, event.c**: Control event handling, etc.
- **main.c**: Program entry.

### LIB Components

- **crypt.h, crypt.c**: Encryption/decryption of LIC and CID.
- **mcid.h, mcid.c**: CID verification, etc.
- **rdx36.h, rdx36.c**: Number (radix 36) and character conversion, etc.
- **slicx.h, slicx.c**: Make LIC.

### Interaction between GUI and LIB

The GUI and LIB interact through the following interface functions:

#### Functions

1. **`int lic_make(lic_t lic, cid_t cid, lic_dev_id dev_id, lic_type_id type_id, uint32_t tick);`**

   - **lic** (out): Encrypted LIC (Output).
   - **cid** (in/out): Encrypted CID (Input) or Decrypted CID (Output).
   - **dev_id** (in): Target device ID.
   - **type_id** (in): License type ID.
   - **tick** (in): The tick value is used to generate random numbers.

---

Please see the source code for more information.

## 2. Encryption and Decryption Algorithms

The encryption/decryption algorithm appears to be customized. The following content is derived from a reverse analysis of the 'uv4.dll' file (ARM MDK v5.37).

### Encryption

#### (1) For Lic

- Compute the start character (one character) and put the character in the start position (LIC[14]).
- Compute the LIC checksum (two characters) and put them in the checksum position (LIC[24], LIC[31]).
- Encrypt LIC with start character (LIC[14]).

#### (2) For CID

- Encrypt CID with start character (CID[07]).

### Decryption

#### (1) For Lic

- Decrypt LIC with start character (CID[14]).
- Verify the checksum (LIC[24], LIC[31]) of the decrypted LIC.
- Verify the start character (CID[14]) of the decrypted LIC.

#### (2) For CID

- Decrypt CID with start character (CID[07]).

The function used to decrypt the LIC can be quickly located by the string "Decrypt LIC", as follows:

```asm
call sub_xxxx1A10 ; --> The target function is sub_xxxx1A10.
mov esi, eax
add esp, 18h
mov eax, offset aOk ; "OK"
test esi, esi
jz short loc_xxxx2E49
mov eax, offset aFailed ; "Failed"
push eax ; char
push offset aDecryptLicS ; "Decrypt LIC... %s"
```

The function used to encrypt the CID can be quickly located by the string "FLF CID:", As follows:

```asm
call sub_xxxx7E10 ; --> The target function is sub_xxxx7E10.
lea eax, [esp+20A0h+var_2063]
push eax ; char
push offset aFlfCidS ; "FLF CID: %s"
```

---

Please see the source code for more information.

## 3. LIC and CID String Description

The following content comes from reverse analysis of "uv4.dll" (ARM MDK v5.37).

### 3.1 CID String Format:

**[00] [01] [02] [03] [04] '-' [06] [07] [08] [09] [10]**

LIC Length (11) == strlen("XXXXX-XXXXX");

The value range of `X` is from '0' to '9' and 'A' to 'Z', and 'O' will be treated as '0'.

Here's the breakdown:

- [00]: Fixed value, 'C'.
- [01]: Related to CPUID.
- [02]: Related to CPUID.
- [03]: Related to CPUID.
- [04]: Related to CPUID.
-----------------------------------------------------------------------------
- [06]: Related to CPUID.
- [07]: Start character for encryption/decryption.
- [08]: Day .
- [09]: Month .
- [10]: Year .

The [08], [09], [10] represent the date when the CID was obtained.

The function used to compute the native CID can be quickly located by the string "Get CID", as follows (Not yet reversed to keygen):

```asm
call sub_xxxx7BE0 ; --> The target function is sub_xxxx7BE0.
lea ecx, [esp+7F8h+var_18]
push ecx ; char
push offset aGetCidS ; "Get CID %s"
```

### 3.2 LIC String Format:

**[00] [01] [02] [03] [04] '-' [06] [07] [08] [09] [10] '-' [12] [13] [14] [15] [16] '-' [18] [19] [20] [21] [22] '-' [24] [25] [26] [27] [28] '-' [30] [31] [32] [33] [34]**

LIC Length (35) == strlen("XXXXX-XXXXX-XXXXX-XXXXX-XXXXX-XXXXX");

The value range of `X` is from '0' to '9' and 'A' to 'Z', and 'O' will be treated as '0'.

Here's the breakdown:

- [00]: License type.
- [01]: Target device.
- [02]: Month (End of Support).
- [03]: Year (End of Support).
- [04]: If LIC[04] == 'T', the operation period is limited to one year. Here, we choose 'C' for a permanent operation period with fewer restrictions (other values were not tested).
-----------------------------------------------------------------------------
- [06]: Sum3 .
- [07]: Random (Usage was not found).
- [08]: Random (Usage was not found).
- [09]: Random (Usage was not found).
- [10]: Random (Usage was not found).
-----------------------------------------------------------------------------
- [12]: Sum1 .
- [13]: If LIC[13] == '0', then devices from all manufacturers are allowed, or restricted to specified manufacturers or specified CPU core devices.
- [14]: Start character for encryption/decryption.
- [15]: Sum2 .
- [16]: Related to the registry, if LIC[04] == 'C', then LIC[16] can only be the following values: 'B', 'C', 'D', 'E', 'U', 'Z' (For more information, see the notes below).
-----------------------------------------------------------------------------
- [18]: Decrypted CID[01].
- [19]: Decrypted CID[02].
- [20]: Decrypted CID[03].
- [21]: Decrypted CID[04].
- [22]: Decrypted CID[06].
-----------------------------------------------------------------------------
- [24]: LIC checksum (HIGH).
- [25]: Related to the registry, if LIC[04] == 'C', then LIC[25] = '0'.
- [26]: Related to the registry, if LIC[04] == 'C', then LIC[26] = '0'.
- [27]: Related to the registry, if LIC[04] == 'C', then LIC[27] = '0'.
- [28]: Related to the registry, if LIC[04] == 'C', then LIC[28] = '0'.
-----------------------------------------------------------------------------
- [30]: Random (Usage was not found).
- [31]: LIC checksum (LOW).
- [32]: Random (Usage was not found).
- [33]: Random (Usage was not found).
- [34]: Random (Usage was not found).

* **Notes: In [16], 'B' stands for Tips from ARM: "E230(1) Embest license and we're not in a Chinese OS".**
* **The function used to writing the registry can be quickly located by the string "Validate LIC and add to registry", as follows:**

```asm
push offset Value ; char
push offset aValidateLicAnd ; "Validate LIC ... to registry..."
push ebp ; int
push 3 ; char
call sub_xxxx1300
lea eax, [esp+520h+var_18]
push eax
lea ecx, [esp+524h+var_4C]
push ecx
call sub_xxxxD420 ; --> The target function is sub_xxxxD420.
```

Functions related to LIC can be found within a critical function, which can be quickly located using any of the following strings (all of which are contained within the critical function):

```
"Perform Verify C"
"Perform Add LIC"
"Read Keil licenses from TOOLS.ini file"
...
```

The results parsed by the critical function may be verified by other modules, such as "uv4.exe", etc (If necessary, we can do memory tracking).

---

Please see the source code for more information.


---------------
## 4 Build and use in shell yourself:
Build:
```
gcc rand.c rdx36.c crypt.c mcid.c slicx.c main.c -o keygen
```

run for help:
```
./keygen -h                             
./keygen: illegal option -- h
Options:

cid: 11 byte Computer ID like XXXXX-XXXXX

dev_id:
 1: C51, 2: C251, 3: C166, 4: ARM, 0: NA

type_id:
 1: Cortex-M0/M0+,
 2: Cortex-M0/M0+ 128K,
 3: Cortex-M0/M0+ 256K,
 4: Macro Assembler Kit,
 5: Compiler/Assembler Kit,
 6: Developers Kit,
 7: Debugger,
 8: Hitex Extensions,
 9: Infineon Extensions,
 10: Essential/Standard Cortex-M only,
 12: Prof. Developers Kit/Plus,
 13: Limited (64K),
 14: Basic (256K)/Community,
 16: RL/Real-Time OS,
 17: Professional Cortex-M only(DS5),
 18: Professional,
 0: NA
```

run:
```
./keygen -c XXXXX-XXXXX -d 1 -t 12
```

shell will return the lic:
```
XXXXX-XXXXX-XXXXX-XXXXX-XXXXX-XXXXX
```