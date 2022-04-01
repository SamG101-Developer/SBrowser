
# SBrowser

---

The SBrowser browser engine is a new browser engine build from the ground up, with its own dom/cssom
implementations, rendering engine, user interface, network stack and storage management. The rendering
and user interface are use the Qt6 graphics library. The network stack is designed with an onion routing
backend, that heavily encrypts and routes traffic across the globe, ensuring privacy and security.

- JavaScript & DOM
  - WebApi objects are all coded from the ground up (no external libraries for implementation)
  - Wrappers around certain V8 objects for ease of access in c++ code
  - V8 JavaScript engine is used for script execution and c++ object integration
  - V8PP used to expose c++ classes into JavaScript and vice-versa


- Rendering Engine
  - Qt6 widget renders for different objects - overriding the virtual qt() method
  - Css integration with regex type and attribute matching with the cssom engine
  - Css applies every attribute (if applicable) in the paint event (executes in < 1/60 second))


- User interface
  - Written in Qt6, with a fresh new layout, differing from other browsers
  - Simple menus that are intuitive and can find items by typing in the focussed menu
  - Every aspect of the UI is completely customizable with respect to colors, sizing, borders etc


- Network stack
  - TOR inspired onion routing technology
  - Every user acts as a routing node
  - Encryption includes
    - AES-256-CTR (symmetric e2e encryption)
    - NTRUEncrypt-HPS-4096-821 (ephemeral keys for key exchange)
    - pqNTRUSign (static keys for signing ephemeral keys)
    - KMAC (keccak based HMAC for integrity verification)
    - SHA3_512 (base hash algorithm for all crypto operations)


- Storage management
  - Sandbox-like environment for reading and writing files (heavy access restriction)


---

## Third party libraries

- V8 - JavaScript execution
- V8PP - Integrate c++ classes into JavaScript
- Qt6 - Graphics Toolkit for all GUI operations
- OpenSSL - Cryptography library

---
