#include "../scan.h"
/* ASN.1 DER encoded length:
 * if (value<=0x80):
 *   emit value;
 * otherwise:
 *   emit 0x80+bytes_that_follow
 *   emit all the bytes in the number, number saved big endian
 * examples:
 *   5 -> 0x05
 *   0xc2 -> 0x81 0xc2
 *   0x123 -> 0x82 0x01 0x23
 *   0x12345 -> 0x83 0x01 0x23 0x45
 */
/* We provide two functions.
 * One that only parses the length value (scan_asn1derlengthvalue), and
 * one that also makes sure that as many bytes as specified by the
 * length are in the input buffer (scan_asn1derlength).
 * If you are trying to parse ASN.1, use scan_asn1derlength.
 * If you just want to use the integer encoding format used by ASN.1 DER
 * for lengths, use scan_asn1derlengthvalue. */

size_t
scan_asn1derlength(const char* src, size_t len, unsigned long long* value) {
  unsigned long long l;
  size_t i = scan_asn1derlengthvalue(src, len, &l);

  if(!i || l > len - i)
    return 0; /* make sure data would fit into buffer */
  *value = l;
  return i;
}
