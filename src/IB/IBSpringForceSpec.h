#ifndef included_IBSpringForceSpec
#define included_IBSpringForceSpec

// Filename: IBSpringForceSpec.h
// Last modified: <11.Apr.2007 04:03:08 boyce@trasnaform2.local>
// Created on 14 Jul 2004 by Boyce Griffith (boyce@trasnaform.speakeasy.net)

/////////////////////////////// INCLUDES /////////////////////////////////////

// IBAMR INCLUDES
#include <ibamr/Stashable.h>

// SAMRAI INCLUDES
#include <tbox/AbstractStream.h>

// C++ STDLIB INCLUDES
#include <vector>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class IBSpringForceSpec provies a mechanism for specifying the data
 * necessary to compute the forces generated by a network of springs.
 *
 * Springs are connections between two particular nodes of the Lagrangian mesh.
 * Forces generated by springs have the feature that the force applied to one
 * end of the spring is the negation of the force applied to the other end of
 * the spring.  Consequently, to determine the total nodal forces, we only need
 * to \em compute the force at one end of the spring (e.g., at the master node).
 * Then, the negation of that force can be applied to the opposite end of the
 * spring (e.g., at the slave node).
 *
 * IBSpringForceSpec objects are stored as Stashable data associated with only
 * the master nodes in the mesh.
 */
class IBSpringForceSpec
    : public Stashable
{
public:
    /*!
     * \brief Register this class and its factory class with the singleton
     * StashableManager object.  This method must be called before any
     * IBSpringForceSpec objects are created.
     *
     * \note This method is collective on all MPI processes.  This is done to
     * ensure that all processes employ the same stashable ID for the
     * IBSpringForceSpec class.
     */
    static void registerWithStashableManager();

    /*!
     * \brief Returns a boolean indicating whether the class has been registered
     * with the singleton StashableManager object.
     */
    static bool getIsRegisteredWithStashableManager();

    /*!
     * \brief Default constructor.
     */
    IBSpringForceSpec(
        const int master_idx=-1,
        const std::vector<int>& slave_idxs=std::vector<int>(),
        const std::vector<int>& force_fcn_idxs=std::vector<int>(),
        const std::vector<double>& stiffnesses=std::vector<double>(),
        const std::vector<double>& rest_lengths=std::vector<double>());

    /*!
     * \brief Virtual destructor.
     */
    virtual ~IBSpringForceSpec();

    /*!
     * \return The number of springs attatched to the master node.
     */
    unsigned getNumberOfSprings() const;

    /*!
     * \return A const refernce to the master node index.
     */
    const int& getMasterNodeIndex() const;

    /*!
     * \return A non-const reference to the master node index.
     */
    int& getMasterNodeIndex();

    /*!
     * \return A const refrence to the slave node indices for the springs
     * attached to the master node.
     */
    const std::vector<int>& getSlaveNodeIndices() const;

    /*!
     * \return A non-const refrence to the slave node indices for the springs
     * attached to the master node.
     */
    std::vector<int>& getSlaveNodeIndices();

    /*!
     * \return A const reference to the force function index of the springs
     * attached to the master node.
     */
    const std::vector<int>& getForceFunctionIndices() const;

    /*!
     * \return A non-const reference to the force function index of the springs
     * attached to the master node.
     */
    std::vector<int>& getForceFunctionIndices();

    /*!
     * \return A const reference to the stiffnesses of the springs attached to
     * the master node.
     */
    const std::vector<double>& getStiffnesses() const;

    /*!
     * \return A non-const reference to the stiffnesses of the springs attached
     * to the master node.
     */
    std::vector<double>& getStiffnesses();

    /*!
     * \return A const reference to the resting length of the springs attached
     * to the master node.
     */
    const std::vector<double>& getRestingLengths() const;

    /*!
     * \return A non-const reference to the resting length of the springs
     * attached to the master node.
     */
    std::vector<double>& getRestingLengths();

    /*!
     * \brief Return the unique identifier used to specify the StashableFactory
     * object used by the StashableManager to extract Stashable objects from
     * data streams.
     */
    virtual int getStashableID() const;

    /*!
     * \brief Return an upper bound on the amount of space required to pack the
     * object to a buffer.
     */
    virtual size_t getDataStreamSize() const;

    /*!
     * \brief Pack data into the output stream.
     */
    virtual void packStream(
        SAMRAI::tbox::AbstractStream& stream);

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    IBSpringForceSpec();

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBSpringForceSpec(
        const IBSpringForceSpec& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBSpringForceSpec& operator=(
        const IBSpringForceSpec& that);

    /*!
     * Indicates whether the factory has been registered with the
     * StashableManager.
     */
    static bool s_registered_factory;

    /*!
     * The stashable ID for this object type.
     */
    static int s_stashable_id;

    /*!
     * Data required to define the spring forces.
     */
    int d_num_springs, d_master_idx;
    std::vector<int> d_slave_idxs, d_force_fcn_idxs;
    std::vector<double> d_stiffnesses, d_rest_lengths;
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibamr/IBSpringForceSpec.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBSpringForceSpec
