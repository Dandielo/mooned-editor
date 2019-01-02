#include <project/interfaces/QProjectElement.h>

#include <QDir>
#include <cassert>

namespace editor
{

QProjectElement::QProjectElement(QProjectElement* parent, QFileInfo element_file) noexcept
    : QObject{ parent }
    , _parent{ parent }
    , _fileinfo{ std::move(element_file) }
{
    // Ensure the directory exist
    _fileinfo.absoluteDir().mkpath(".");
}

auto QProjectElement::name() const noexcept -> QString
{
    return _fileinfo.baseName();
}

auto QProjectElement::location() const noexcept -> QDir
{
    return _fileinfo.absoluteDir();
}

auto QProjectElement::identifier() const noexcept -> QString
{
    return _fileinfo.absoluteFilePath();
}

auto QProjectElement::project() noexcept -> QProject*
{
    return _parent->project();
}

auto QProjectElement::project() const noexcept -> const QProject*
{
    return _parent->project();
}

} // namespace editor
