/**
 * \mainpage libcfp \doc_type Documentation
 *
 * \section abstract Abstract
 *
 * \b libcfp is a library for <b>C</b>hemical <b>F</b>ormula
 * <b>P</b>arsing .
 *
 * - C++
 * - syntax only
 * - no semantics
 * - no information about real world element characteristics
 * - no additional libraries or other dependencies for usage, namespace ::std:: only
 * - uses UnitTest++ to ensure consistent behaviour
 *
 * The ASCII representation of a chemical formula is analysed. For every
 * element within the following characteristics are extracted:
 *
 * - symbol name (first letter in upper case, all following letters in lower
 *   case)
 * - nucleon number (is isotope or not, optional, integer number)
 * - coefficient (optional, floating point number unequal 1.0)
 *
 * Following formulas are valid and synonymical:
 *
 * - "H3.4C(13)5"
 * - "H3.4 (13C)5.0"
 * - "H3.4 [13C]5.0"
 * - "H3.4 13C5.0"
 *
 * As result, the empirical formula is generated:
 *
 * - "K3(J(4))2.3(13C)1.2" \f$ \rightarrow \f$ "(13C)1.2 (4J)2.3 K3"
 * - "1H5 1H7" \f$ \rightarrow \f$ "(1H)12"
 * - "1H 5 H7" \f$ \rightarrow \f$ "H7 (1H)5"
 * - "K3(J(4)2(J4(K2.2(F3J))3F2.3))2.5" \f$ \rightarrow \f$ "F28.25 J17.5 (4J)5 K19.5"
 *
 * The formatted string representation of a formula can also be generated (for the
 * last example):
 *
 * - K<sub>3</sub>(<sup>4</sup>J<sub>2</sub>(J<sub>4</sub>(K<sub>2.2</sub>(F<sub>3</sub>J))<sub>3</sub>F<sub>2.3</sub>))<sub>2.5</sub>
 *
 * \section docinfo Further Documentation
 *
 * \if doc_user
 * See the <a href="doc_full/index.html">Full Documentation</a> for developer 
 * oriented documentation of the implementation details which is not intended 
 * for application developers.
 * \endif
 * \if doc_full
 * See the <a href="../index.html">User Documentation</a> for user oriented
 * documentation (application developers).
 * \endif
 *
 * \section license License and Copyright Information
 *
 * <table border="0">
 * <tr><td>
 * - This library is released under the 
 * <a href="http://www.gnu.org/licenses/lgpl.html">GNU Lesser General Public License</a> (LGPL).
 * </td><td>
 * \image html logo_lgpl.png
 * \image latex logo_lgpl.png width=2cm
 * </td></tr><tr><td>
 * - Initially, it was written by Ingo Bressler (ingob at users.berlios.de) at the
 * <a href="http://www.chemie.tu-berlin.de/gradzielski/menue/physikalische_chemie_molekulare_materialwissenschaften/">Stranski-Laboratory for Physical and Theoretical Chemistry</a> of the <a href="http://www.tu-berlin.de">Technische Universit&auml;t Berlin</a>.
 * </td><td>
 * \image html logo_tub.png
 * \image latex logo_tub.png width=2cm
 * </td></tr><tr><td>
 * - It is hosted by <a href="http://developer.berlios.de">BerliOS</a>.
 * </td><td>
 * \htmlonly
 * <a href="http://developer.berlios.de" title="BerliOS Developer"><img src="http://developer.berlios.de/bslogo.php?group_id=11425" width="124px" height="32px" border="0" alt="BerliOS Developer Logo"></a>
 * \endhtmlonly
 * </td></tr>
 * </table>
 *
 */

