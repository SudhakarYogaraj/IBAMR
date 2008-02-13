#ifndef included_INSProjectionBcCoef
#define included_INSProjectionBcCoef

// Filename: INSProjectionBcCoef.h
// Last modified: <13.Feb.2008 13:29:15 griffith@box221.cims.nyu.edu>
// Created on 22 Feb 2007 by Boyce Griffith (boyce@trasnaform2.local)

/////////////////////////////// INCLUDES /////////////////////////////////////

// STOOLS INCLUDES
#include <stools/ExtendedRobinBcCoefStrategy.h>

// C++ STDLIB INCLUDES
#include <vector>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class INSProjectionBcCoef is a concrete
 * SAMRAI::solv::RobinBcCoefStrategy that is used to specify boundary conditions
 * for the solution of the discrete Poisson problem that must be solved in the
 * implementation of a projection method.
 */
class INSProjectionBcCoef
    : public virtual STOOLS::ExtendedRobinBcCoefStrategy
{
public:
    /*!
     * \brief Constructor.
     *
     * \param P_idx            Patch data descriptor index for the cell-centered pressure field.
     * \param P_bc_coef        Boundary condition specification object corresponding to the pressure.
     * \param projection_type  The type of projection ("pressure_increment" or "pressure_update").
     * \param u_idx            Patch data descriptor index for the face-centered intermediate velocity field.
     * \param u_bc_coefs       Vector of boundary condition specification objects corresponding to the components of the velocity.
     * \param homogeneous_bc   Whether to employ homogeneous (as opposed to inhomogeneous) boundary conditions
     *
     * \note Precisely NDIM boundary condition objects must be provided to the
     * class constructor.
     */
    INSProjectionBcCoef(
        const int P_idx,
        SAMRAI::solv::RobinBcCoefStrategy<NDIM>* const P_bc_coef,
        const std::string& projection_type,
        const int u_idx,
        const std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*>& u_bc_coefs,
        const bool homogneous_bc=false);

    /*!
     * \brief Destructor.
     */
    virtual
    ~INSProjectionBcCoef();

    /*!
     * \brief Reset the problem coefficents required to specify the boundary
     * conditions for the scalar function phi.
     */
    void
    setProblemCoefs(
        const double rho,
        const double dt);

    /*!
     * \brief Reset the patch data descriptor index for the cell-centered
     * pressure.
     *
     * \param P_idx  Patch data descriptor index for the cell-centered pressure field.
     */
    void
    setCurrentPressurePatchDataIndex(
        const int P_idx);

    /*!
     * \brief Set the SAMRAI::solv::RobinBcCoefStrategy objects used to specify
     * physical boundary conditions for the pressure.
     *
     * \param P_bc_coef  Boundary condition specification object corresponding to the pressure.
     */
    void
    setPressurePhysicalBcCoef(
        SAMRAI::solv::RobinBcCoefStrategy<NDIM>* const P_bc_coef);

    /*!
     * \brief Set the type of projection to perform ("pressure_increment" or
     * "pressure_update").
     *
     * \param projection_type  The type of projection ("pressure_increment" or "pressure_update").
     */
    void
    setProjectionType(
        const std::string& projection_type);

    /*!
     * \brief Reset the patch data descriptor index for the face-centered
     * intermediate velocity.
     *
     * \param u_idx  Patch data descriptor index for the face-centered intermediate velocity field.
     */
    void
    setIntermediateVelocityPatchDataIndex(
        const int u_idx);

    /*!
     * \brief Set the SAMRAI::solv::RobinBcCoefStrategy objects used to specify
     * physical boundary conditions for the velocity.
     *
     * \param u_bc_coefs  Vector of boundary condition specification objects corresponding to the components of the velocity.
     */
    void
    setVelocityPhysicalBcCoefs(
        const std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*>& u_bc_coefs);

    /*!
     * \name Implementation of STOOLS::ExtendedRobinBcCoefStrategy interface.
     */
    //\{

    /*!
     * \brief Set the target data index.
     */
    virtual void
    setTargetPatchDataIndex(
        const int target_idx);

    /*!
     * \brief Set whether the class is filling homogeneous or inhomogeneous
     * boundary conditions.
     */
    virtual void
    setHomogeneousBc(
        const bool homogeneous_bc);

    //\}

    /*!
     * \name Implementation of SAMRAI::solv::RobinBcCoefStrategy interface.
     */
    //\{

    /*!
     * \brief Function to fill arrays of Robin boundary condition coefficients
     * at a patch boundary.
     *
     * \note In the original SAMRAI::solv::RobinBcCoefStrategy interface, it was
     * assumed that \f$ b = (1-a) \f$.  In the new interface, \f$a\f$ and
     * \f$b\f$ are independent.
     *
     * \see SAMRAI::solv::RobinBcCoefStrategy::setBcCoefs()
     *
     * \param acoef_data  Boundary coefficient data.
     *        The array will have been defined to include index range for
     *        corresponding to the boundary box \a bdry_box and appropriate for
     *        the alignment of the given variable.  If this is a null pointer,
     *        then the calling function is not interested in a, and you can
     *        disregard it.
     * \param bcoef_data  Boundary coefficient data.
     *        This array is exactly like \a acoef_data, except that it is to be
     *        filled with the b coefficient.
     * \param gcoef_data  Boundary coefficient data.
     *        This array is exactly like \a acoef_data, except that it is to be
     *        filled with the g coefficient.
     * \param variable    Variable to set the coefficients for.
     *        If implemented for multiple variables, this parameter can be used
     *        to determine which variable's coefficients are being sought.
     * \param patch       Patch requiring bc coefficients.
     * \param bdry_box    Boundary box showing where on the boundary the coefficient data is needed.
     * \param fill_time   Solution time corresponding to filling, for use when coefficients are time-dependent.
     */
    virtual void
    setBcCoefs(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& acoef_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& bcoef_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& gcoef_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> >& variable,
        const SAMRAI::hier::Patch<NDIM>& patch,
        const SAMRAI::hier::BoundaryBox<NDIM>& bdry_box,
        double fill_time=0.0) const;

    /*
     * \brief Return how many cells past the edge or corner of the patch the
     * object can fill.
     *
     * The "extension" used here is the number of cells that a boundary box
     * extends past the patch in the direction parallel to the boundary.
     *
     * Note that the inability to fill the sufficient number of cells past the
     * edge or corner of the patch may preclude the child class from being used
     * in data refinement operations that require the extra data, such as linear
     * refinement.
     *
     * The boundary box that setBcCoefs() is required to fill should not extend
     * past the limits returned by this function.
     */
    virtual SAMRAI::hier::IntVector<NDIM>
    numberOfExtensionsFillable() const;

    //\}

protected:

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    INSProjectionBcCoef();

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    INSProjectionBcCoef(
        const INSProjectionBcCoef& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    INSProjectionBcCoef&
    operator=(
        const INSProjectionBcCoef& that);

    /*!
     * \brief Implementation of boundary condition filling function.
     */
    void
    setBcCoefs_private(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& acoef_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& bcoef_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> >& gcoef_data,
        const SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> >& variable,
        const SAMRAI::hier::Patch<NDIM>& patch,
        const SAMRAI::hier::BoundaryBox<NDIM>& bdry_box,
        double fill_time) const;

    /*
     * The patch data index corresponding to the current pressure.
     */
    int d_P_idx;

    /*
     * The physical boundary conditions for the pressure (only used at "open"
     * boundaries where the pressure is specified).
     */
    SAMRAI::solv::RobinBcCoefStrategy<NDIM>* d_P_bc_coef;

    /*
     * The type of projection (pressure-increment or pressure-update).
     */
    std::string d_projection_type;

    /*
     * The patch data index corresponding to the intermediate velocity.
     */
    int d_u_idx;

    /*
     * The boundary condition specification objects for the updated velocity.
     */
    std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*> d_u_bc_coefs;

    /*
     * Whether to use homogeneous boundary conditions.
     */
    bool d_homogeneous_bc;

    /*
     * Fluid density.
     */
    double d_rho;

    /*
     * Timestep size.
     */
    double d_dt;
};
}// namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibamr/INSProjectionBcCoef.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_INSProjectionBcCoef
