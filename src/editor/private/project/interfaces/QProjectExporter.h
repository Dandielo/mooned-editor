#pragma once

namespace editor
{

//! The project base class.
class QProject;

//! The project element class.
class QProjectElement;

//! A exporter interface for QProject objects.
class QProjectExporter
{
public:
    //! virtual dtor
    virtual ~QProjectExporter() noexcept = default;

    //! Serializes a given project.
    virtual void export_project(QProject* project) const noexcept = 0;

    //! Serializes a given graph.
    virtual void export_project_element(QProjectElement* project) const noexcept = 0;
};

} // namespace editor
